# RISCV OS

## Tagline
Lightweight educational operating system with cooperative multithreading and time-sharing support.

## Overview
**RISCV OS** is a minimalistic, educational operating system kernel designed for the **RISC-V RV64IMA architecture**. It provides a platform for learning core OS concepts such as **thread management, memory allocation, and synchronization**, while keeping the implementation accessible and easy to understand.

This project emphasizes **cooperative multithreading**, where threads voluntarily yield control to allow other threads to execute. 

RISCV OS is implemented in **C++**, with optional low-level **RISC-V assembly** for hardware interactions and runs entirely in a single, unified address space.

---

## Features

- **Cooperative Multithreading**: 
  - Create, terminate and switch between threads.
  - Threads explicitly yield control; no asynchronous preemption.
  - Support for periodic and sleeping threads.

- **Memory Management**: 
  - Simple heap allocator for dynamic memory allocation.
  - Efficient block-based memory management suitable for embedded environments.

- **Synchronization Primitives**:
  - Semaphore implementation for coordinating threads.
  - Thread-safe operations within the kernel.

- **Console I/O**:
  - Basic input/output API for user applications.
  - Provides a simple interface for testing and debugging.

---

## Architecture

RISCV OS is structured in **layers**:

1. **Kernel Layer**  
   Handles core operations such as thread scheduling, memory management, and hardware abstraction.

2. **C++ API Layer**  
   Provides object-oriented interfaces for threads, semaphores, and I/O operations.  
   Example classes: `Thread`, `Semaphore`, `PeriodicThread`, `Console`.

3. **C API Layer**  
   Procedural functions wrapping system calls, enabling compatibility with simpler programs.

4. **ABI (Application Binary Interface)**  
   Low-level interface for system calls and hardware access via software interrupts.

All layers work within a single address space. The kernel executes in privileged mode, while user threads execute in unprivileged mode, with cooperative scheduling.

---

## Tech Stack

- **Programming Languages**: C++, RISC-V Assembly  

---

## Project Highlights

- Designed and implemented a **cooperative multithreading kernel** from scratch.
- Developed a **lightweight memory allocator** for embedded OS environments.
- Built **synchronization primitives** (semaphores) for safe thread interaction.
- Created **object-oriented API** for user-level thread management.
- Implemented **console I/O** for easy debugging and testing.
- Fully **simulator-compatible**, running in standard RISC-V environments.

---
