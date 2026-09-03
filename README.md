# DWM3001C UCI Firmware

Firmware **UCI** para el dispositivo DWM3001CDK basado en el [Qorvo DW3 QM33 SDK v1.1.1](https://github.com/qorvo/QM33_SDK).

## 📋 Descripción del Proyecto

Este proyecto es una modificación del firmware **UCI** del SDK QM33 v1.1.1 configurado específicamente para:

- **Interfaz**: USB (CDC ACM vía J-Link integrado) **habilitada por defecto** (`-DUSB_ENABLE` en `Projects/FreeRTOS/UCI/DWM3001CDK/project_UCI.cmake`)
- **Protocolo**: UCI (UWB Control Interface — protocolo binario de control definido por la FIRA/Qorvo, en contraposición al protocolo CLI de consola de texto)
- **Target**: DWM3001CDK (nRF52833)
- **Firmware**: UCI FreeRTOS

> El repositorio también incluye los targets alternativos **CLI** (`Projects/FreeRTOS/CLI/`) y **QANI** (`Projects/FreeRTOS/QANI/`), disponibles como referencia pero **no son el firmware que usa este proyecto**. El proyecto hermano [`i-mop-qorvo-cli-fw`](https://github.com/EstebanMenti/i-mop-qorvo-cli-fw) mantiene la variante CLI.

## 🎯 Firmware Compilado

El firmware compilado se encuentra en:

```
BuildOutput/UCI/FreeRTOS/DWM3001CDK/Release/DWM3001CDK-UCI-FreeRTOS.hex
```

### ✅ Verificación del Firmware

El firmware UCI se verifica con las herramientas del repositorio [`uwb-qorvo-tools/`](uwb-qorvo-tools/) (scripts Python de Qorvo para el protocolo UCI), por ejemplo consultando la información del dispositivo:

```bash
python uwb-qorvo-tools/scripts/device/get_device_info/get_device_info.py --port COM22
```

También pueden utilizarse los scripts auxiliares `uqt_info` y `uqt_ls`, o `decode_uci` para decodificar tráfico UCI crudo.

## 📂 Estructura del Repositorio

```
i-mop-qorvo-uci-fw/
├── BuildOutput/              # Directorio de compilación (generado, ignorado por git)
├── docs/                     # Documentación del proyecto (reglas de trabajo)
├── Libs/                     # Librerías del proyecto
│   ├── dwt_uwb_driver/      # Driver UWB Qorvo (DW3000)
│   ├── niq/                 # Librerías NIQ (Qorvo)
│   ├── uwb-stack/           # UWB Stack (Qorvo)
│   └── uwbstack_libs/       # Librerías precompiladas del stack UWB
├── Projects/                 # Proyectos y sistema de build
│   ├── Common/              # Cmakefiles y scripts comunes (CreateTargetCommon.py)
│   └── FreeRTOS/
│       ├── UCI/             # Firmware UCI (el que usa este proyecto)
│       │   ├── Common/      # Código común del target UCI
│       │   ├── DWM3001CDK/  # Configuración DWM3001CDK (nRF52833)
│       │   ├── nRF52840DK/  # Configuración nRF52840DK
│       │   └── Type2AB_EVB/ # Configuración Type2AB EVB
│       ├── CLI/             # Firmware CLI (alternativa, no usada por este proyecto)
│       ├── HelloWorld/      # Proyecto de ejemplo base
│       └── QANI/            # Firmware QANI (alternativa, no usada por este proyecto)
├── SDK_BSP/                  # BSP del Nordic SDK
│   └── Nordic/SDK_17_1_0/   # Nordic nRF5 SDK v17.1.0
├── Src/                      # Código fuente del proyecto
│   ├── AppConfig/           # Configuración de la aplicación
│   ├── Apps/                # Aplicaciones (uci, listener, fira, reporter, common)
│   ├── Boards/              # Configuración de placas (DWM3001CDK, nRF52840DK, Type2AB_EVB)
│   ├── Comm/                # Capa de comunicación (USB, UART, BLE)
│   ├── EventManager/        # Gestor de eventos
│   ├── HAL/                 # Hardware Abstraction Layer (nrfx)
│   ├── Helpers/             # Utilidades (buffers circulares, cJSON, conversión UWB, debug)
│   ├── Logger/              # Procesamiento de logs
│   ├── OS/                  # FreeRTOS (kernel)
│   ├── OSAL/                # OS Abstraction Layer
│   └── UWB/                 # Capa UWB (frames, traducción MCPS, utilidades)
├── uwb-qorvo-tools/          # Herramientas UCI de Qorvo (scripts Python de verificación)
├── .vscode/                  # Configuración de VS Code
│   └── scripts/             # Scripts de utilidad (flash_target.py, set_config.py)
├── BUILD.md                  # 📖 Instrucciones de compilación
├── LICENSES/                 # Licencias del proyecto
└── README.md                 # Este archivo
```

## 🚀 Inicio Rápido

### Requisitos Previos

- ARM Toolchain (arm-none-eabi-gcc 10.3)
- CMake (≥ 3.23)
- Python 3.10+
- Make / MinGW
- SEGGER J-Link

Ver [BUILD.md](BUILD.md) para instrucciones detalladas de instalación.

### Compilación Rápida

```bash
# 1. Crear entorno virtual
python -m venv .venv
.venv\Scripts\Activate.ps1  # Windows

# 2. Instalar dependencias
pip install -r requirements.txt

# 3. Crear target y compilar (Release)
cd Projects/FreeRTOS/UCI/DWM3001CDK
python CreateTarget.py -build Release
cd ../../../BuildOutput/UCI/FreeRTOS/DWM3001CDK/Release
make -j
```

El firmware compilado estará en:

```
BuildOutput/UCI/FreeRTOS/DWM3001CDK/Release/DWM3001CDK-UCI-FreeRTOS.hex
```

### Flashear el Firmware

Usar SEGGER J-Link o VS Code (ver [BUILD.md](BUILD.md#flasheo-del-firmware)):

```bash
# Opción 1: script incluido
flash_dwm3001cdk.bat

# Opción 2: script Python
python .vscode/scripts/flash_target.py
```

## 📖 Documentación

| Documento | Descripción |
|-----------|-------------|
| [BUILD.md](BUILD.md) | Instrucciones detalladas de configuración y compilación |
| [docs/README.md](docs/README.md) | Índice de la documentación del proyecto |
| [docs/CONTRIBUTING.md](docs/CONTRIBUTING.md) | Reglas de trabajo: Git, commits, pull requests, documentación |

## 🔧 Configuración del Proyecto

### Interfaz USB / UCI

El firmware UCI tiene la interfaz **USB habilitada por defecto** (`-DUSB_ENABLE`, `Projects/FreeRTOS/UCI/DWM3001CDK/project_UCI.cmake`) — el protocolo UCI (binario) se transporta por el puerto COM virtual (CDC ACM) expuesto por el J-Link integrado.

- `Src/Comm/` — Capa de comunicación (USB y UART)
- `Src/Apps/Src/uci/` — Parser UCI, transporte y tareas UCI
- `Projects/FreeRTOS/UCI/DWM3001CDK/` — Configuración específica para DWM3001CDK (target UCI)

### Placas Soportadas

| Placa | CPU | Estado |
|-------|-----|--------|
| DWM3001CDK | nRF52833 | ✅ Activo |
| Type2AB EVB | nRF52840 | ⚠️ Experimental |
| nRF52840DK | nRF52840 | ⚠️ Experimental |

## 🛠️ Desarrollo

### VS Code

El proyecto incluye configuración para VS Code:

- IntelliSense para C/C++
- Tasks para compilar y flashear (`Ctrl+Shift+B`, seleccionar configuración `UCI` / `DWM3001CDK` / `Release`)
- Launch configuration para debugging
- Extensiones recomendadas

Ver [BUILD.md](BUILD.md) para configuración detallada.

### Scripts de Utilidad

Scripts disponibles en `.vscode/scripts/`:

- `flash_target.py` - Flashear firmware al dispositivo
- `set_config.py` - Configurar parámetros del proyecto

## 📚 Recursos Adicionales

- [Qorvo Developer Portal](https://www.qorvo.com/products/development/software/qorvo-uwb-devel-software)
- [Nordic nRF5 SDK Documentation](https://infocenter.nordicsemi.com/)
- [DWM3001CDK User Guide](https://www.qorvo.com/products-development-tools/dwm3001c-development-kit)
- [uwb-qorvo-tools (GitHub)](https://github.com/qorvo/uwb-qorvo-tools)

## 📄 Licencia

Este proyecto se basa en el QM33 SDK de Qorvo. Ver directorio `LICENSES/` para información completa de licencias.

## ⚠️ Notas

- Este proyecto modifica el firmware original del SDK QM33 v1.1.1
- El protocolo UCI es binario (framed); para interactuar con el dispositivo usar los scripts de `uwb-qorvo-tools/`, no un terminal serie de texto
- La interfaz USB (CDC ACM) está habilitada por defecto en el target UCI para DWM3001CDK

---

**Basado en**: Qorvo DW3 QM33 SDK v1.1.1  
**Firmware**: UCI FreeRTOS  
**Interfaz**: USB (CDC ACM) por defecto  
**Última actualización**: 2026-09-03
