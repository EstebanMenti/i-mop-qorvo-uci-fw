# Instrucciones de Compilación

Este documento describe cómo compilar y flashear el firmware **UCI** del DWM3001C (el repositorio también incluye los targets alternativos CLI y QANI, no usados por este proyecto; ver [README.md](README.md)). Para una visión general del proyecto, ver [README.md](README.md). Para la referencia exhaustiva de todos los parámetros de compilación (herramientas, versiones, variables de entorno, scripts, flags), ver [docs/BUILD_PARAMETERS.md](docs/BUILD_PARAMETERS.md).

## 📋 Tabla de Contenidos

- [Herramientas Requeridas](#herramientas-requeridas)
- [Configuración del Entorno](#configuración-del-entorno)
- [Compilación desde VS Code](#compilación-desde-visual-studio-code)
- [Compilación desde Línea de Comandos](#compilación-desde-línea-de-comandos)
- [Flasheo del Firmware](#flasheo-del-firmware)
- [Salida de la Compilación](#salida-de-la-compilación)
- [Solución de Problemas](#solución-de-problemas)

---

## Herramientas Requeridas

Las siguientes herramientas son necesarias para compilar y ejecutar el firmware:

### Windows

| Herramienta | Versión | Descarga | Ruta de instalación |
|------|---------|---------|--------------|
| **ARM Toolchain** | 10.3-2021.10 | [ARM Developer](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) | `C:\GnuToolsArmForEmbedded\gcc-arm-none-eabi-10.3-2021.10\bin` |
| **CMake** | ≥ 3.23 | [CMake.org](https://cmake.org/download/) | `<cmake_install_dir>` |
| **Python** | ≥ 3.10 | [Python.org](https://www.python.org/downloads/) | (Agregar al PATH durante la instalación) |
| **MinGW + Make** | Latest | [SourceForge](https://sourceforge.net/projects/mingw/) | `<install path>/bin` |
| **SEGGER J-Link** | Latest | [SEGGER](https://www.segger.com/downloads/jlink/) | Instalación por defecto |
| **VS Code** | Latest | [VSCode](https://code.visualstudio.com/) | Instalación por defecto |

#### Detalles de instalación en Windows

**ARM Toolchain:**
1. Descargar `arm-none-eabi-gcc-10.3-2021.10-win32`
2. Instalar en: `C:\GnuToolsArmForEmbedded\gcc-arm-none-eabi-10.3-2021.10\bin`
3. Agregar al PATH: `C:\GnuToolsArmForEmbedded\gcc-arm-none-eabi-10.3-2021.10\bin`

**MinGW y Make:**
1. Instalar MinGW usando MinGW Installation Manager
2. Instalar el paquete `mingw32-base`
3. Copiar `<install path>/bin/mingw32-make.exe` como `<install path>/bin/make.exe`
4. Agregar `<install path>/bin` al PATH

### Linux

| Herramienta | Comando de instalación |
|------|---------------------|
| **ARM Toolchain** | Ver abajo |
| **Make** | `sudo apt-get install build-essential` |
| **CMake** | Ver abajo |
| **Python** | `sudo apt-get install -y python3 python3-pip` |
| **SEGGER J-Link** | [SEGGER](https://www.segger.com/downloads/jlink/) |
| **VS Code** | [VSCode](https://code.visualstudio.com/) |

#### Detalles de instalación en Linux

**ARM Toolchain:**
```bash
mkdir /opt/gcc
cd /opt/gcc
wget https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.10/gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2
tar -xvf gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2
rm gcc-arm-none-eabi-10.3-2021.10-x86_64-linux.tar.bz2
```

Agregar a `~/.bashrc`:
```bash
export PATH="/opt/gcc/gcc-arm-none-eabi-10.3-2021.10/bin:${PATH}"
```

**CMake:**
```bash
wget https://github.com/Kitware/CMake/releases/download/v3.27.0/cmake-3.27.0-linux-x86_64.sh
sudo mkdir /usr/bin/cmake
sudo cmake-3.27.0-linux-x86_64.sh --skip-license --prefix=/usr/bin/cmake
```

Agregar a `~/.bashrc`:
```bash
export PATH="/usr/bin/cmake/bin:${PATH}"
```

---

## Configuración del Entorno

### 1. Crear Entorno Virtual

```bash
python -m venv .venv
```

### 2. Activar Entorno Virtual

**Linux y macOS:**
```bash
source .venv/bin/activate
```

**Windows:**
```bash
.venv\Scripts\Activate.ps1
```

### 3. Instalar Requerimientos

```bash
pip install -r requirements.txt
```

---

## Compilación desde Visual Studio Code

El proyecto provee soporte para compilar, flashear y depurar el firmware directamente desde VS Code.

### ⚠️ Notas Importantes

> **Advertencia:** Las rutas con espacios generan errores. Ubicar siempre este repositorio en un directorio sin espacios.

> **Advertencia:** Asegurarse de que VS Code use el intérprete de Python del entorno virtual. Ejecutar `Ctrl+Shift+P`, escribir `Python: Select interpreter`, y elegir `./.venv/bin/python` (o `.\.venv\Scripts\python.exe` en Windows).

> **Advertencia:** En Windows, el terminal recomendado es PowerShell. Ejecutar `Ctrl+Shift+P`, escribir `Terminal: Select Default Profile`, y elegir **PowerShell**.

### Abrir el Workspace

1. Clic en `File` en el menú superior
2. Elegir `Open Workspace from File...`
3. Buscar y abrir `DW3_QM33_SDK.code-workspace`

### Extensiones

Instalar las extensiones recomendadas:

1. Ir a la pestaña de extensiones (`Ctrl+Shift+X`)
2. Clic en el botón **Filter extensions**
3. Elegir **Recommended**
4. Instalar todas:

| Extensión | Propósito |
|-----------|---------|
| `ms-vscode.cpptools` | Soporte del lenguaje C/C++ |
| `marus25.cortex-debug` | Depuración de firmware |
| `rioj7.command-variable` | Tasks de build automatizadas |
| `ms-python.python` | Soporte del lenguaje Python |

### Tasks Disponibles

| Task | Descripción |
|------|-------------|
| **Build the firmware** | Compilar solo los archivos modificados |
| **Build the clean firmware** | Recompilar todo desde cero |
| **Flash the target** | Flashear el firmware y resetear |
| **Build & flash the target** | Compilar + Flashear |
| **Debug the firmware** | Sesión de depuración GDB |
| **Build & debug the firmware** | Compilar + Depurar |

### Ejecución de Tasks

1. Abrir el diálogo de build (`Ctrl+Shift+B`)
2. Elegir **Choose a configuration** y seleccionar:
   - **Build**: `UCI`
   - **Board**: `DWM3001CDK`
   - **Config**: `Release` (o `Debug`)
3. Ejecutar las tasks desde la paleta de comandos o el terminal

---

## Compilación desde Línea de Comandos

### 1. Crear el Target

`CreateTarget.py` automatiza la configuración de CMake.

**Navegar a la carpeta del proyecto:**
```bash
cd Projects/FreeRTOS/UCI/DWM3001CDK
```

**Ejecutar el script:**

**Linux:**
```bash
./CreateTarget.py [-no-force] [-build] [-custom-flags]
```

**Windows:**
```bash
python ./CreateTarget.py [-no-force] [-build] [-custom-flags]
```

#### Opciones

| Opción | Descripción |
|--------|-------------|
| `-no-force` | No elimina la carpeta de build anterior |
| `-build {type}` | Tipo de build: `Debug`, `Release`, `RelWithDebInfo`, `MinSizeRel`, `Custom` |
| `-custom-flags FLAGS` | Flags personalizados (solo con `-build Custom`) |

#### Ejemplos

```bash
# Build Release
python ./CreateTarget.py -build Release

# Build personalizado con flags
python ./CreateTarget.py -build Custom -custom-flags='-O2 -g -DDEBUG'
```

### 2. Compilar el Firmware

Esto compila el firmware, produciendo los archivos `.hex`, `.bin` y `.elf`.

**Navegar al directorio de build:**
```bash
cd BuildOutput/UCI/FreeRTOS/DWM3001CDK/Release
```

**Compilar:**
```bash
make -j
```

**Archivos de salida esperados:**
```
DWM3001CDK-UCI-FreeRTOS.hex
DWM3001CDK-UCI-FreeRTOS.bin
DWM3001CDK-UCI-FreeRTOS.elf
DWM3001CDK-UCI-FreeRTOS.map
```

> **Nota:** Usar el flag `-j` para compilación en paralelo (usa todos los núcleos de CPU). Usar `-j4` para limitar a 4 núcleos.

### 3. Ejemplos de Build

```bash
# Target UCI para DWM3001CDK, build Release (usado por este proyecto)
cd BuildOutput/UCI/FreeRTOS/DWM3001CDK/Release
make -j

# Target UCI alternativo para Type2AB EVB, build Debug
cd BuildOutput/UCI/FreeRTOS/Type2AB_EVB/Debug
make -j
```

---

## Flasheo del Firmware

### Usando el Script Incluido (Windows)

El repositorio incluye un script de flasheo rápido para la DWM3001CDK:

```bash
flash_dwm3001cdk.bat
```

Este script ejecuta J-Link con la configuración de [flash_dwm3001cdk.jlink](flash_dwm3001cdk.jlink) y guarda la salida en `flash_output.txt`.

### Usando J-Link por Línea de Comandos

1. **Crear `script.jlink`:**

```txt
si 1
speed 4000
device <cpu_type>
loadfile <firmware_path>
r
g
exit
```

2. **Reemplazar los valores:**

| Parámetro | Valor para DWM3001CDK | Valor para nRF52840DK / Type2AB EVB |
|-----------|---------------------|---------------------|
| `<cpu_type>` | `nrf52833_xxaa` | `nrf52840_xxaa` |
| `<firmware_path>` | `BuildOutput/UCI/FreeRTOS/DWM3001CDK/Release/DWM3001CDK-UCI-FreeRTOS.hex` | `BuildOutput/UCI/FreeRTOS/<BOARD>/Debug/<BOARD>-UCI-FreeRTOS.hex` |

3. **Ejecutar J-Link:**

**Linux:**
```bash
JLinkExe -CommanderScript script.jlink
```

**Windows:**
```bash
JLink.exe -CommanderScript script.jlink
```

### Usando VS Code

1. Abrir el diálogo de build (`Ctrl+Shift+B`)
2. Seleccionar la configuración (UCI/DWM3001CDK/Release)
3. Ejecutar la task **Flash the target**

### Usando el Script Python

```bash
python .vscode/scripts/flash_target.py
```

---

## Salida de la Compilación

El firmware compilado se encuentra en:

```
BuildOutput/UCI/FreeRTOS/DWM3001CDK/Release/
├── DWM3001CDK-UCI-FreeRTOS.hex    # Formato Intel HEX (para flashear)
├── DWM3001CDK-UCI-FreeRTOS.bin    # Binario crudo
├── DWM3001CDK-UCI-FreeRTOS.elf    # Ejecutable ELF (para depuración)
└── DWM3001CDK-UCI-FreeRTOS.map    # Mapa de memoria
```

---

## Solución de Problemas

### Problemas Comunes

| Problema | Solución |
|-------|----------|
| **"arm-none-eabi-gcc not found"** | Agregar la ARM toolchain al PATH |
| **"CMake not found"** | Instalar CMake ≥ 3.23 y agregarlo al PATH |
| **"Python not found"** | Instalar Python 3.10+ y agregarlo al PATH |
| **"make not found"** | Instalar MinGW (Windows) o build-essential (Linux) |
| **El build falla por espacios en la ruta** | Mover el proyecto a una ruta sin espacios |
| **Las tasks de VS Code no funcionan** | Verificar el perfil de terminal (PowerShell en Windows) |

---

Para más información, ver:
- [README.md](README.md) — Visión general del proyecto
- [docs/CONTRIBUTING.md](docs/CONTRIBUTING.md) — Reglas de trabajo (Git, documentación)
