# Referencia de Parámetros de Compilación

> Documento de referencia exhaustivo de **todos los parámetros** que intervienen en la compilación del firmware **UCI** para la placa **DWM3001CDK**: herramientas y versiones, variables de entorno, scripts y sus argumentos, variables de CMake, definiciones del compilador, tasks de VS Code y artefactos generados. Para una guía paso a paso de instalación y uso, ver [`BUILD.md`](../BUILD.md). Para el flujo de trabajo con Git, ver [`CONTRIBUTING.md`](CONTRIBUTING.md).

## 📋 Tabla de Contenidos

- [Resumen de la configuración verificada](#resumen-de-la-configuración-verificada)
- [Herramientas y versiones](#herramientas-y-versiones)
- [Variables de entorno](#variables-de-entorno)
- [Scripts de build y sus parámetros](#scripts-de-build-y-sus-parámetros)
- [Variables de CMake](#variables-de-cmake)
- [Definiciones del compilador y flags](#definiciones-del-compilador-y-flags)
- [Configuración del target UCI](#configuración-del-target-uci)
- [Tasks de VS Code e inputs](#tasks-de-vs-code-e-inputs)
- [Matriz de targets disponibles](#matriz-de-targets-disponibles)
- [Artefactos de salida y uso de memoria](#artefactos-de-salida-y-uso-de-memoria)
- [Registro de compilación verificada](#registro-de-compilación-verificada)

---

## Resumen de la configuración verificada

| Parámetro | Valor |
|---|---|
| Firmware | `DWM3001CDK-UCI-FreeRTOS` |
| Sistema operativo embebido | FreeRTOS |
| MCU | Nordic nRF52833 (Cortex-M4F, 128 KB RAM / 512 KB FLASH) |
| Toolchain | GNU Arm Embedded 10.3-2021.10 (`arm-none-eabi-gcc`) |
| Sistema de build | CMake + Make |
| Script de configuración | [`CreateTarget.py`](../Projects/FreeRTOS/UCI/DWM3001CDK/CreateTarget.py) |
| Script de target CMake | [`project_UCI.cmake`](../Projects/FreeRTOS/UCI/DWM3001CDK/project_UCI.cmake) |
| Toolchain file de CMake | [`arm-none-eabi-gcc.cmake`](../Projects/Common/cmakefiles/arm-none-eabi-gcc.cmake) |
| Tipo de build por defecto | `Release` (usado por este proyecto) |
| Carpeta de build | `BuildOutput/UCI/FreeRTOS/DWM3001CDK/Release` |

---

## Herramientas y versiones

Versiones efectivamente utilizadas y verificadas en Windows 11 (ver [Registro de compilación verificada](#registro-de-compilación-verificada)):

| Herramienta | Versión requerida | Versión verificada | Comando de verificación |
|---|---|---|---|
| GNU Arm Embedded Toolchain | 10.3-2021.10 | 10.3.1 20210824 (release) | `arm-none-eabi-gcc --version` |
| CMake | ≥ 3.23 | 4.3.4 | `cmake --version` |
| GNU Make | ≥ 3.82 | 3.82.90 (MinGW) | `make --version` |
| Python | ≥ 3.10 | 3.12.10 | `python --version` |
| MinGW (Windows) | Latest | — | provee `make.exe` |
| SEGGER J-Link | Latest | — | solo para flasheo |
| ccache (opcional) | Latest | no instalado | acelera recompilaciones |

Ruta de instalación del toolchain usada en la verificación:

```text
C:\Program Files (x86)\GNU Arm Embedded Toolchain\10 2021.10\bin\arm-none-eabi-gcc.exe
```

---

## Variables de entorno

| Variable | Obligatoria | Valor / propósito |
|---|---|---|
| `PYTHONPATH` | **Sí** (fuera de VS Code) | Debe apuntar a `Projects/Common/scripts` para que `CreateTarget.py` pueda importar `CreateTargetCommon.py`. Las tasks de VS Code no lo necesitan porque internamente resuelven la importación. |
| `TOOLCHAIN_PREFIX` | No | Prefijo del cross-compilador. Por defecto `arm-none-eabi-`. Permite usar un toolchain con otro nombre de prefijo (ver [`arm-none-eabi-gcc.cmake`](../Projects/Common/cmakefiles/arm-none-eabi-gcc.cmake)). |
| `USE_CCACHE` | No | Si se define como `0`, deshabilita el uso de ccache aunque esté instalado. Si no está definido y ccache existe, se habilita automáticamente. |

### Cómo definir `PYTHONPATH` (PowerShell)

```powershell
cd Projects/FreeRTOS/UCI/DWM3001CDK
$env:PYTHONPATH = "../../Common/scripts"
python CreateTarget.py -build Release
```

---

## Scripts de build y sus parámetros

### `CreateTarget.py` (por target)

Ubicación: [`Projects/FreeRTOS/UCI/DWM3001CDK/CreateTarget.py`](../Projects/FreeRTOS/UCI/DWM3001CDK/CreateTarget.py)

Valores fijos que instancia para este proyecto: `target_os = "FreeRTOS"`, `board = "DWM3001CDK"`, `sample = "UCI"`.

### `CreateTargetCommon.py` (común)

Ubicación: [`Projects/Common/scripts/CreateTargetCommon.py`](../Projects/Common/scripts/CreateTargetCommon.py)

Contiene el parser de argumentos y genera el comando `cmake`:

| Argumento | Valores | Default | Descripción |
|---|---|---|---|
| `-build` | `Debug`, `Release`, `RelWithDebInfo`, `MinSizeRel`, `Custom` | `Debug` | Tipo de build; se propaga como `CMAKE_BUILD_TYPE`. Con `Custom` no se define `CMAKE_BUILD_TYPE`. |
| `-no-force` | (flag) | desactivado | No elimina la carpeta de build previa (incremental). Sin este flag la carpeta `BuildOutput/<sample>/<os>/<board>/<build>` se borra por completo (build limpio). |
| `-custom-flags` | string | vacío | Flags personalizados del compilador C; **solo** válido junto a `-build Custom`. Ejemplo: `'-O2 -g -DDEBUG'`. Se propaga como `CMAKE_C_FLAGS`. |

Comportamiento interno relevante:

- Generador: `MinGW Makefiles` en Windows, `Unix Makefiles` en Linux/macOS.
- Ruta de build resultante: `<PROJECT_BASE>/BuildOutput/<sample>/<os>/<board>/<build>`.
- Si la ruta de build existe y no se pasa `-no-force`, se elimina recursivamente antes de configurar.

### `flash_target.py`

Ubicación: [`.vscode/scripts/flash_target.py`](.vscode/scripts/flash_target.py)

Usado por las tasks de VS Code para flashear el `.hex` vía J-Link:

| Argumento | Descripción |
|---|---|
| `<ruta .hex>` | Ruta absoluta del firmware a flashear (formato `<board>-<sample>-FreeRTOS.hex`). |
| `<cpu>` | Tipo de CPU para J-Link (ejemplo: `nrf52833`). |

### `set_config.py`

Ubicación: [`.vscode/scripts/set_config.py`](.vscode/scripts/set_config.py)

Guarda la configuración seleccionada en [`.vscode/project_config.json`](.vscode/project_config.json):

```json
{
    "example": "UCI",
    "board": "DWM3001CDK",
    "cpu": "nrf52833",
    "build": "Release",
    "flags": ""
}
```

| Campo | Valores posibles |
|---|---|
| `example` | `UCI`, `CLI`, `QANI`, `HelloWorld` |
| `board` | `nRF52840DK`, `DWM3001CDK`, `Type2AB_EVB` |
| `cpu` | `nrf52833` (DWM3001CDK), `nrf52840` (nRF52840DK), `nrf52833` (Type2AB_EVB) |
| `build` | `Debug`, `Release`, `RelWithDebInfo`, `MinSizeRel`, `Custom` |
| `flags` | string libre (solo con `build: Custom`) |

---

## Variables de CMake

Variables pasadas por `CreateTargetCommon.py` en la invocación de `cmake -S <fuente> -B <build>`:

| Variable | Valor (para este proyecto) | Propósito |
|---|---|---|
| `-S` | `Projects/FreeRTOS/UCI/DWM3001CDK` | Directorio fuente del target. |
| `-B` | `BuildOutput/UCI/FreeRTOS/DWM3001CDK/<build>` | Directorio de build. |
| `-G` | `MinGW Makefiles` / `Unix Makefiles` | Generador según SO. |
| `CMAKE_TOOLCHAIN_FILE` | `Projects/Common/cmakefiles/arm-none-eabi-gcc.cmake` | Define el cross-compilador ARM. |
| `MY_TARGET_SCRIPT` | `Projects/FreeRTOS/UCI/DWM3001CDK/project_UCI.cmake` | Script de configuración específica del target. |
| `PROJECT_BASE` | raíz del repositorio | Base absoluta del proyecto. |
| `COMMON_PATH` | `Projects/Common/cmakefiles` | Ruta de los cmake files comunes. |
| `PROJECT_COMMON` | `Projects/FreeRTOS/UCI/Common` | Código fuente común al sample (ej. `main.c`, `hooks.c`). |
| `LIBS_PATH` | `Libs` | Ruta de las librerías precompiladas. |
| `CMAKE_BUILD_TYPE` | `Release` (omitido con `Custom`) | Tipo de build estándar de CMake. |
| `CMAKE_C_FLAGS` | solo con `-build Custom` | Flags C personalizados. |

Variables definidas por el toolchain file ([`arm-none-eabi-gcc.cmake`](../Projects/Common/cmakefiles/arm-none-eabi-gcc.cmake)):

| Variable | Valor |
|---|---|
| `CMAKE_SYSTEM_NAME` | `Generic` |
| `CMAKE_SYSTEM_PROCESSOR` | `ARM` |
| `CMAKE_C_COMPILER` | `${TOOLCHAIN_PREFIX}gcc` |
| `CMAKE_ASM_COMPILER` | igual al compilador C |
| `CMAKE_CXX_COMPILER` | `${TOOLCHAIN_PREFIX}g++` |
| `CMAKE_OBJCOPY` | `${TOOLCHAIN_PREFIX}objcopy` |
| `CMAKE_SIZE_UTIL` | `${TOOLCHAIN_PREFIX}size` |
| `CMAKE_TRY_COMPILE_TARGET_TYPE` | `STATIC_LIBRARY` |

---

## Definiciones del compilador y flags

Definidas en [`project_UCI.cmake`](../Projects/FreeRTOS/UCI/DWM3001CDK/project_UCI.cmake):

### Flags obligatorios (`CMAKE_CUSTOM_C_FLAGS`)

| Flag | Propósito |
|---|---|
| `-Werror` | Trata todos los warnings como errores. |
| `-DBOARD_CUSTOM` | Define la placa como custom (DWM3001CDK no es HDK estándar). |
| `-DUSB_ENABLE` | Habilita el transporte USB del protocolo UCI. |
| `-DCONFIG_GPIO_AS_PINRESET` | Usa el pin GPIO como RESET. |

### Otras definiciones

| Definición | Valor | Propósito |
|---|---|---|
| `add_definitions(-Os)` | — | Optimización por tamaño aplicada globalmente. |
| `add_definitions(-DUSE_USB_ENUM_WORKAROUND)` | — | Workaround para enumeración USB lenta. **Debe quitarse si el HDK se alimenta por batería.** |
| `USE_CRYPTO_BACKEND_MBEDTLS` | `1` | Usa mbedTLS como backend criptográfico. |
| `USE_CRYPTO_SPEED_OPTIMIZATION` | `0` | Optimización de velocidad de crypto deshabilitada. |
| `USE_DRV_DW3000` | `1` | Compila el driver DW3000 desde fuentes. |
| `USE_DRV_DW3720` | `0` | Driver DW3720 deshabilitado. |

### Arquitectura y FPU

| Variable | Valor |
|---|---|
| `PROJECT_ARCH` | `m4` (Cortex-M4) |
| `PROJECT_FP` | `hard` (hard-float ABI) |
| `PROJECT_FPU` | `fpv4-sp-d16` |
| `MY_CPU` | `NRF52833_XXAA` |
| Linker script | `nRF52833.ld` |

---

## Configuración del target UCI

| Variable | Valor | Descripción |
|---|---|---|
| `MY_BOARD` | `DWM3001CDK` | Placa destino. |
| `MY_OS` | `FreeRTOS` | Sistema operativo. |
| `MY_SAMPLE` | `UCI` | Aplicación/protocolo (Unify Control Interface). |
| `MY_HAL` | `nrfx` | Capa HAL usada (ver [Src/HAL/Src/nrfx](../Src/HAL/Src/nrfx)). |
| `MY_BSP` | `Nordic` | Board Support Package (ver [SDK_BSP/Nordic](../SDK_BSP/Nordic)). |
| `MY_TARGET` | `DWM3001CDK-UCI-FreeRTOS` | Nombre final del firmware. |

Stack UWB utilizado: **uwbstack bundle `full`** (incluye `uci_bundle`), librerías precompiladas en [Libs/uwbstack_libs/delivery/full](../Libs/uwbstack_libs/delivery/full) para `arm-cortex-m4`/`m33` (versión `R12.7.0-00405-gb33c5c42726c`).

---

## Tasks de VS Code e inputs

Definidas en [`.vscode/tasks.json`](.vscode/tasks.json):

| Task | Comando efectivo | Descripción |
|---|---|---|
| Install the requirements | `pip install -r requirements.txt` | Instala dependencias Python. |
| Choose a configuration | `set_config.py ...` | Guarda `example`/`board`/`build`/`flags` en `project_config.json`. |
| Check the configuration | `echo` | Muestra la configuración activa. |
| Build the firmware | `make all V=1 -j` en `BuildOutput/<example>/FreeRTOS/<board>/<build>` | Configura (si falta) y compila incrementalmente. |
| Build the clean firmware | idem, previo `_force create target` | Borra la carpeta de build y recompila todo. |
| Flash the target | `flash_target.py <hex> <cpu>` | Flashea vía J-Link (requiere firmware ya compilado). |
| Build & flash the target | Build + `flash_target.py` | Compila y flashea. |
| Debug the firmware | sesión GDB (ver [`launch.json`](.vscode/launch.json)) | Depuración con Cortex-Debug. |

Inputs disponibles en los prompts (`Ctrl+Shift+B` → *Choose a configuration*):

| Input | Opciones | Default |
|---|---|---|
| Example type | `UCI`, `CLI`, `QANI`, `HelloWorld` | `UCI` |
| Board | `nRF52840DK`, `DWM3001CDK`, `Type2AB_EVB` | `nRF52840DK` |
| Build type | `Debug`, `Release`, `RelWithDebInfo`, `MinSizeRel`, `Custom` | `Debug` |
| Custom flags | string libre | vacío |

> **Configuración usada por este proyecto:** `Build: UCI`, `Board: DWM3001CDK`, `Build type: Release`.

---

## Matriz de targets disponibles

Combinaciones con `CreateTarget.py` en el repositorio (este proyecto solo usa la primera):

| Sample | Board | Estado en este proyecto |
|---|---|---|
| UCI | DWM3001CDK | ✅ **Target principal** (verificado) |
| UCI | Type2AB_EVB | Alternativo |
| UCI | nRF52840DK | Alternativo |
| CLI | DWM3001CDK / nRF52840DK / Type2AB_EVB | No usado |
| QANI | DWM3001CDK / nRF52840DK / Type2AB_EVB / Q-TAG | No usado |
| HelloWorld | DWM3001CDK / nRF52840DK | No usado |

---

## Artefactos de salida y uso de memoria

Archivos generados en `BuildOutput/UCI/FreeRTOS/DWM3001CDK/Release`:

| Archivo | Descripción |
|---|---|
| `DWM3001CDK-UCI-FreeRTOS.elf` | Binario ELF con símbolos (depuración). |
| `DWM3001CDK-UCI-FreeRTOS.hex` | Intel HEX para flasheo con J-Link. |
| `DWM3001CDK-UCI-FreeRTOS.bin` | Binario plano. |
| `DWM3001CDK-UCI-FreeRTOS.map` | Mapa de memoria y símbolos. |

Uso de memoria del build verificado (Release):

| Región | Usado | Total | % |
|---|---|---|---|
| RAM | 107.420 B | 128 KB | 81,95 % |
| FLASH | 256.572 B | 504 KB | 49,71 % |
| CALIB_SHA | 32 B | 4 KB | 0,78 % |
| CALIB | 4 KB | 4 KB | 100 % |

---

## Registro de compilación verificada

| Campo | Valor |
|---|---|
| Fecha | 2026-09-03 |
| Sistema | Windows 11, terminal cmd/PowerShell |
| Configuración | `UCI` / `DWM3001CDK` / `Release` |
| Procedimiento | `python CreateTarget.py -build Release` + `make -j` |
| Resultado | Build 100 % sin errores ni warnings (con `-Werror`) |
| Commit de verificación | `d1a9649` (build: add prebuilt vendor libraries required for UCI firmware build) |

Comandos ejecutados (PowerShell/cmd, desde la raíz del repositorio):

```bat
cd Projects\FreeRTOS\UCI\DWM3001CDK
set "PYTHONPATH=<raiz>\Projects\Common\scripts"
python CreateTarget.py -build Release
cd ..\..\..\BuildOutput\UCI\FreeRTOS\DWM3001CDK\Release
make -j
```
