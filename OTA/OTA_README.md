# Automotive OTA Update State Machine

A reference implementation of a secure Over-The-Air (OTA) update mechanism for automotive ECU systems, written in C++14.

> **Note:** This is an educational reference implementation that simulates automotive OTA architecture. TrustZone, secure boot, and partition operations are simulated to illustrate the design — this is not a production implementation.

---

## Why This Exists

Updating software in a car is nothing like updating software on a phone.

A phone has one processor, one OS, one software stack. A modern vehicle has 100+ ECUs — each with its own software, memory constraints, boot sequences, and safety-critical dependencies.

And unlike a phone:

- A failed update **cannot just restart and try again**
- Some ECUs control braking, steering, and airbags
- The update pipeline must guarantee a safe, recoverable state — **every single time**

This project models the core architecture that makes safe automotive OTA possible: a state machine with A/B partition switching, cryptographic verification via TrustZone, secure boot validation, anti-rollback protection, health checking, and automatic rollback on failure.

---

## Architecture Overview

```
┌─────────────────────────────────────────────────────────────┐
│                    OTA State Machine                        │
│                                                             │
│   ┌────────┐    ┌──────────┐    ┌──────────┐                │
│   │  Idle  │───▶│ Download │───▶│  Verify  │                │
│   └────────┘    └──────────┘    └──────────┘                │
│                                      │                      │
│                              ┌───────┴────────┐             │
│                              ▼                ▼             │
│                          ┌───────┐       ┌────────┐         │
│                          │Install│       │ Error  │         │
│                          └───────┘       └────────┘         │
│                              │                              │
│                              ▼                              │
│                          ┌────────┐                         │
│                          │ Reboot │                         │
│                          └────────┘                         │
│                              │                              │
│                              ▼                              │
│                     ┌─────────────────┐                     │
│                     │  Secure Boot    │                     │
│                     │  Verification   │                     │
│                     └────────┬────────┘                     │
│                              │                              │
│                              ▼                              │
│                          ┌────────┐                         │
│                          │ Health │                         │
│                          │ Check  │                         │
│                          └────────┘                         │
│                              │                              │
│                 ┌────────────┴────────────┐                 │
│                 ▼                         ▼                 │
│            ┌─────────┐            ┌──────────┐              │
│            │ Success │            │ Rollback │              │
│            └─────────┘            └──────────┘              │
└─────────────────────────────────────────────────────────────┘
```

---

## The A/B Partition Model

One of the most important concepts in automotive OTA is the **A/B partition model** — and it's the reason a failed update doesn't brick the vehicle.

```
┌─────────────────────────────────────────────┐
│              ECU Flash Memory               │
│                                             │
│  ┌──────────────┐    ┌──────────────┐       │
│  │  Partition A │    │  Partition B │       │
│  │  (Active)    │    │  (Inactive)  │       │
│  │              │    │              │       │
│  │  Running     │    │  New update  │       │
│  │  software    │    │  written     │       │
│  │  v1.0        │    │  here → v2.0 │       │
│  └──────────────┘    └──────────────┘       │
│                                             │
│  If health check passes → switch to B       │
│  If health check fails  → stay on A         │
└─────────────────────────────────────────────┘
```

The new software is always written to the **inactive partition**. The vehicle continues running on the active partition throughout the download and install phases. Only after a successful reboot and health check does the system commit to the new partition. If anything fails — the vehicle silently rolls back to the last known good state.

---

## State Descriptions

### `IdleState`
The starting point. The state machine waits here until an update is triggered with a version number and OEM signature. In a real vehicle, this state listens for update notifications from the cloud backend (e.g., via MQTT or HTTPS polling).

### `DownloadState`
The update package is downloaded to a staging area. In production, this involves chunk-based download with resumability — a network dropout at 98% should not require restarting from zero. Download integrity is verified via checksum before advancing.

### `VerifyState`
The most security-critical state. Two checks happen here:

**1. Cryptographic signature verification (TrustZone / TEE)**
The update package must carry a valid OEM signature. This is verified inside the Trusted Execution Environment — isolated from the normal OS, inaccessible to malware running in the main software stack. If the signature doesn't match the OEM public key stored in secure storage, the update is rejected immediately.

**2. Anti-rollback protection**
The new version number must be strictly greater than the currently installed version. This prevents an attacker from downgrading the ECU to an older version with known vulnerabilities — even if they have a validly signed older package.

```
Failure in VerifyState → ErrorState (update rejected, no changes made)
```

### `InstallState`
The verified update package is written to the **inactive partition**. The active partition — and the running vehicle software — is untouched throughout this operation. If the vehicle loses power during installation, the active partition remains intact and the vehicle boots normally on next startup.

### `RebootState`
The ECU reboots and attempts to boot from the newly installed partition. The bootloader performs **secure boot verification** before handing control to the new software — ensuring the installed image hasn't been tampered with between install and boot.

### `HealthState`
After booting from the new partition, the system runs health checks. In a real implementation, this includes:
- Critical process startup verification
- Communication stack availability checks
- Safety function self-tests
- A configurable watchdog timeout — if health checks don't complete within the window, rollback is triggered automatically

```
Health check passes → SuccessState (partition switch committed)
Health check fails  → RollbackState (revert to previous partition)
```

### `SuccessState`
The update is committed. The new partition becomes the active partition. Version number in secure storage is updated. The OEM cloud backend is notified of successful installation.

### `RollbackState`
The system reverts to the previously active partition. The failed update is logged. The vehicle returns to normal operation on the known-good software version. In production, the OEM backend receives a failure report with diagnostic data to investigate root cause.

### `ErrorState`
Terminal failure state — reached when verification fails (invalid signature or version downgrade attempt). No changes are made to the ECU. The update is rejected cleanly and the incident is logged for security analysis.

---

## Security Architecture

```
┌─────────────────────────────────────────────────────┐
│                   Security Layers                   │
│                                                     │
│  ┌───────────────────────────────────────────────┐  │
│  │           TrustZone / TEE (VerifyState)       │  │
│  │                                               │  │
│  │  • Signature verification against OEM key     │  │
│  │  • Anti-rollback version check                │  │
│  │  • Isolated from normal OS — tamper-proof     │  │
│  └───────────────────────────────────────────────┘  │
│                                                     │
│  ┌───────────────────────────────────────────────┐  │
│  │         Secure Boot (RebootState)             │  │
│  │                                               │  │
│  │  • Verifies installed image before boot       │  │
│  │  • Root of trust anchored in hardware         │  │
│  │  • Blocks execution of tampered software      │  │
│  └───────────────────────────────────────────────┘  │
│                                                     │
│  ┌───────────────────────────────────────────────┐  │
│  │         SecureStorage                         │  │
│  │                                               │  │
│  │  • OEM public key storage                     │  │
│  │  • Current version tracking                   │  │
│  │  • Protected from normal-world access         │  │
│  └───────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────┘
```

---

## Three Scenarios Demonstrated

The `main.cpp` runs three scenarios that cover the critical OTA failure paths:

**Scenario 1 — Successful update**
```
Version: 2, Signature: valid OEM key
Flow: Idle → Download → Verify ✓ → Install → Reboot → Health ✓ → Success
Result: Partition switched, version committed
```

**Scenario 2 — Version downgrade attempt (Anti-rollback)**
```
Version: 1, Signature: valid OEM key
Flow: Idle → Download → Verify ✗ (version not newer) → Error
Result: Update rejected, no changes made
```

**Scenario 3 — Tampered update package (Invalid signature)**
```
Version: 3, Signature: invalid/hacked
Flow: Idle → Download → Verify ✗ (signature mismatch) → Error
Result: Update rejected, security incident logged
```

---

## Project Structure

```
OTA/
├── main.cpp                    # Three OTA scenarios
├── ota_state.h                 # OTAState interface (pure virtual)
├── ota_state_machine.h/.cpp    # Context — owns current state
├── states/
│   ├── idle_state.h/.cpp       # Entry point, awaits update trigger
│   ├── download_state.h/.cpp   # Package download
│   ├── verify_state.h/.cpp     # TrustZone verification + anti-rollback
│   ├── install_state.h/.cpp    # Write to inactive partition
│   ├── reboot_state.h/.cpp     # Reboot + secure boot
│   ├── health_state.h/.cpp     # Post-boot health checks
│   ├── success_state.h/.cpp    # Commit new partition
│   ├── rollback_state.h/.cpp   # Revert to previous partition
│   └── error_state.h/.cpp      # Terminal failure
└── security/
    ├── trustzone.h/.cpp        # TEE signature + anti-rollback simulation
    ├── bootloader.h/.cpp       # Secure boot verification simulation
    └── secure_storage.h/.cpp   # OEM key and version storage
```

---

## Build & Run

**Requirements:** C++14 compatible compiler (g++ or clang++)

**Build:**
```bash
g++ -std=c++14 main.cpp ota_state_machine.cpp states/*.cpp security/*.cpp -o ota
```

**Run:**
```bash
./ota
```

**Expected output:**
```
--> State: Idle
--> State: Download
Downloading version 2...
--> State: Verify
TrustZone: Verifying signature...
TrustZone: Verification successful.
--> State: Install
Installing to partition B
--> State: Reboot
--> State: Health
Health check...
--> State: Success

--> State: Idle
--> State: Download
Downloading version 1...
--> State: Verify
TrustZone: Verifying signature...
TrustZone: Anti-rollback triggered.
----- OTA Update Failed: Version 1 is not newer than current version 2 -----
--> State: Error

--> State: Idle
--> State: Download
Downloading version 3...
--> State: Verify
TrustZone: Verifying signature...
TrustZone: Signature mismatch.
----- OTA Update Failed: Signature verification failed -----
--> State: Error
```

---

## Design Pattern

This implementation uses the **State design pattern** (GoF). The `OTAStateMachine` is the context — it holds a pointer to the current state and delegates all behaviour to it. Each concrete state owns its transition logic, making it trivial to add new states or modify transitions without touching other states.

This pattern maps naturally onto real automotive OTA implementations where each state has distinct entry conditions, actions, and exit paths — and where safety requirements demand that transitions be explicit, traceable, and testable in isolation.

---

## What This Doesn't Cover

This is a reference implementation. A production automotive OTA system would additionally include:

- **Delta updates** — sending only the changed bytes, not the full image (critical for bandwidth-constrained vehicles)
- **Multi-ECU orchestration** — coordinating updates across 100+ ECUs in dependency order
- **Campaign management** — OEM cloud backend scheduling, targeting, and monitoring update rollouts across a vehicle fleet
- **AUTOSAR UCM (Update and Configuration Management)** — the AUTOSAR standard module that governs OTA in AUTOSAR Adaptive platforms
- **Bandwidth throttling and resume** — chunk-based download with persistent resume across ignition cycles
- **Consent and scheduling UI** — driver notification and approval flow

---

## Author

**Utsav Krishna**
Automotive Software Engineer | Product Owner | Tech Lead
Nearly a decade building ECU software across infotainment, V2X, cybersecurity, OTA, and SDV tooling.

[LinkedIn](https://www.linkedin.com/in/utsav-krishna) | [utsavk.contact@gmail.com]

---

## License

MIT — free to use, learn from, and build on.
