# Smart Light (SmaL)

A low-cost, multiplatform **Visible Light Communication (VLC)** receiver prototype for indoor attocell downlink communication. The system consists of a Plug-and-Play (PnP) USB hardware dongle, the **SmaL PnP**, paired with a browser-based web application, the **SmaL Web**, for reactive data acquisition and visualization.

> 📹 [Watch the demo on YouTube](https://www.youtube.com/watch?v=HrTFufJpvJ0)

## Overview

As the Radio Frequency (RF) spectrum grows increasingly congested, VLC emerges as a promising complementary technology for indoor environments, leveraging existing LED infrastructure for energy-efficient communication.

This project implements a complete VLC downlink receiver chain:

```
LED Transmitter > Optical Channel > SmaL PnP (USB Dongle) > SmaL Web (Browser)
```

The receiver achieves **error-free communication** over distances from **30 cm to 280 cm** under Line-of-Sight (LOS) conditions, and was validated in a hospital monitoring use case using the [HL7](https://www.hl7.org/) standard for patient vital sign transmission.

## Repository Structure

This repository contains the **receiver-side implementation**, composed of:

| Component | Description |
|:---|:---|
| `SmaL PnP` | Embedded firmware for the SAMD21 microcontroller |
| `SmaL Web` | Vue.js/Quasar web application for serial data decoding and visualization |

## System Architecture

### Transmitter (reference setup)
- A **Raspberry Pi** receives patient data from a multiparameter monitor via Ethernet (HL7 protocol)
- Data is encoded with **Manchester coding** and modulated using **On-Off Keying (OOK)**
- The modulated signal drives an **LED**, converting it to an optical signal

### Receiver (this repository)
1. **OPT101 photosensor** converts incident optical power to an analog electrical signal
2. **Low-pass filter** (< 120 Hz) generates an ambient LED lighting interference reference signal 
3. **Comparator** reconstructs a clean digital waveform from the filtered reference
4. **SAMD21** decodes Manchester-encoded bits, validates packets via parity check, and transmits data over USB (UART/serial)
5. **SmaL Web** reads the serial stream via the [Web Serial API](https://developer.mozilla.org/en-US/docs/Web/API/Web_Serial_API), decodes IEEE 754 floating-point values, and reactively displays them.

### Packet Protocol

```
[ HEADER (8 bits) | FP1 (32 bits) | ... | FPN (32 bits) | Parity (1 bit) ]
```

Payload values are encoded in **IEEE 754 floating-point** format. The parity bit is set to `1` when the number of `1` bits in the payload is even, and `0` otherwise.

## Features

- 🔌 **Plug-and-Play** USB dongle, no specialized hardware required on the host
- 🌐 **Multiplatform** web interface, runs in any Chromium-based browser (Chrome, Edge, etc)
- 💡 **Ambient light rejection** via low-pass filtering and comparator-based signal recovery
- 📡 **Manchester decoding** with interrupt-driven firmware on the SAMD21
- 📊 **Reactive visualization** of received physiological parameters
- ✅ **BER evaluation**, zero error achieved up to 280 cm under LOS conditions
- 🏥 **HL7-compatible**, validated with a multiparameter patient monitor

## Performance

| Condition | Distance | BER |
|:---|:---|:---|
| No ambient light | 30 to 280 cm | ~0% |
| Ambient light, high gain (Rf = 860 kΩ) | up to 300 cm | < 0.26% |
| Ambient light, low gain (Rf = 157 kΩ) | 30 to 290 cm | ~0% (2.45% at 300 cm) |

Goodput (N = 6 parameters): **~955 bps** (~4.48% protocol overhead), at ~5 packets/second.

## Requirements

### Hardware
- SAMD21-based board (e.g., Arduino Zero or compatible)
- OPT101 photosensor
- Comparator and low-pass filter circuit (see schematic in paper)

> **Note for Windows users:** Installing the [Adafruit Windows Drivers](https://github.com/adafruit/Adafruit_Windows_Drivers/releases/tag/2.5.0.0) is required to use the SAMD21 over USB.

### Software
- [Node.js](https://nodejs.org/) and [Quasar CLI](https://quasar.dev/) (for the web app)
- A Chromium-based browser with Web Serial API support

## Getting Started

```bash
# Clone the repository
git clone https://github.com/nascimentolaf/SmaL.git
cd SmaL

# Install web app dependencies
cd SmaL\ Web/
npm install

# Run the development server
npx quasar dev
```

Then open the app in Chrome/Edge, plug in the SmaL PnP dongle, and click **"Select Port"** to begin receiving data.

## Meta

| | |
|:---|:---|
| **Authors** | Alaf Nascimento, Klaas Zwaag, Wesley Costa, Higor Camporez, Francisco Santos, Jair Silva |
| **Affiliation** | Electrical Engineering Dept., Federal University of Espírito Santo (UFES), Vitória/ES, Brazil |
| **Development Year** | 2022 |
| **License** | [MIT](LICENSE) |

## Publication

If you use this work, please cite:

```bibtex
@inproceedings{nascimento2022smal,
  title     = {A Low-Cost Multiplatform VLC System Prototype for Indoor Attocell Downlink Communication},
  author    = {Nascimento, Alaf and Zwaag, Klaas and Costa, Wesley and Camporez, Higor and Santos, Francisco and Silva, Jair},
  booktitle = {[Conference name]},
  year      = {2022},
  address   = {Vitória, ES, Brazil}
}
```

> The first author is currently affiliated with LTCI, Télécom Paris, Institut Polytechnique de Paris, France.