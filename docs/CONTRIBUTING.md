# CONTRIBUTING — Flujo de trabajo con Git y documentación

> **Propósito de este documento:** definir cómo se rama, se commitea, se documenta y se indexa el trabajo en este repositorio (firmware DWM3001C **UCI**, basado en el Qorvo DW3 QM33 SDK v1.1.1; incluye también los targets alternativos CLI y QANI, no usados por este proyecto). Leerlo antes de abrir una rama, hacer un commit o agregar un documento nuevo.

---

## 1. Ramas

- `main`: siempre estable; solo recibe merges por pull request. Debe compilar y flashear correctamente en la DWM3001CDK.
- Ramas de trabajo con el formato `<tipo>/<descripcion-corta-kebab-case>`:
  - `feature/` — nueva funcionalidad (ej.: `feature/uci-session-config`)
  - `fix/` — corrección de errores (ej.: `fix/uci-frame-parsing`)
  - `docs/` — solo documentación
  - `chore/` — mantenimiento, tooling, build system, CI
  - `refactor/` — reestructuración sin cambio de comportamiento

---

## 2. Commits — Conventional Commits en español

Formato: `<tipo>(<ámbito opcional>): <descripción en imperativo, minúscula, sin punto final>`

- **Tipos permitidos:** `feat`, `fix`, `docs`, `test`, `refactor`, `chore`, `perf`.
- **Ámbitos sugeridos** (según la estructura del repo):
  - `uci` — firmware y protocolo UCI (`Projects/FreeRTOS/UCI/`, `Src/Apps/Src/uci/`), el usado por este proyecto
  - `apps` — otras aplicaciones (`Src/Apps/Src/listener`, `fira`, `reporter`, `common`)
  - `uwb` — capa `Src/UWB` (frames, traducción MCPS, utilidades)
  - `drivers` / `hal` — `Src/HAL`, `Src/Boards` (drivers de placa, periféricos)
  - `bsp` — `SDK_BSP` (paquete de soporte de placa de Nordic/Qorvo)
  - `comm` — `Src/Comm` (UART, USB, BLE, transporte)
  - `os` / `osal` — `Src/OS`, `Src/OSAL` (FreeRTOS, capa de abstracción de OS)
  - `build` — sistema de build, `Projects/`, scripts de flasheo (`flash_dwm3001cdk.*`)
  - `tools` — `uwb-qorvo-tools` (scripts Python auxiliares de verificación UCI)
  - `docs` — documentación
- La descripción va **en español**. Ejemplos:
  - `feat(uci): agrega soporte para el comando SESSION_GET_STATUS`
  - `fix(comm): corrige el desalineado de tramas USB al reconectar`
  - `chore(build): actualiza el script de flasheo para DWM3001CDK`
  - `docs: documenta el procedimiento de verificación con get_device_info`
- Commits atómicos: un cambio lógico por commit. No mezclar refactor con feature.
- No commitear: código comentado muerto, binarios generados fuera de `BuildOutput/`, credenciales, `.venv/`, logs ni artefactos de build intermedios.

---

## 3. Pull requests

- Todo cambio a `main` pasa por PR, incluso siendo un solo desarrollador (deja trazabilidad).
- El título del PR sigue el mismo formato que los commits.
- La descripción debe incluir: **qué** cambia, **por qué**, **cómo se probó** (build local, flasheo real en DWM3001CDK, salida de `get_device_info.py` u otro script de `uwb-qorvo-tools/` cuando aplique), y logs o capturas de salida relevantes.
- Requisitos para mergear: el firmware compila sin warnings nuevos, se verificó en hardware real cuando el cambio lo amerita, y la documentación está actualizada si el cambio la afecta.
- Merge por *squash* si la rama tiene commits de corrección intermedios; merge normal si los commits son atómicos y valiosos.

---

## 4. Reglas de documentación

- **Idioma:** toda la documentación del proyecto se redacta **en español**.
- **Formato:** Markdown — títulos numerados, tablas para datos enumerables, bloques `> **Nota/Advertencia**` para avisos, bloques de código para comandos y salidas.
- Todo documento nuevo en `docs/` debe: (a) empezar con un encabezado que indique propósito y alcance, (b) agregarse al índice [`docs/README.md`](README.md).
- Las afirmaciones sobre el comportamiento del SDK o del firmware deben citar la fuente (Qorvo DW3 QM33 SDK, especificación UCI de FIRA, documentación oficial del fabricante). Lo que no proviene de una fuente verificada se marca **[Sin verificar]**.
- El `README.md` de la raíz se mantiene sincronizado con la realidad del firmware: si cambia el procedimiento de build, flasheo o verificación, se actualiza en el mismo pull request.

---

## 5. Reglas para asistentes de IA

1. **No inventar comportamiento del SDK ni del firmware.** Ante una duda de protocolo o de driver, citar la fuente (SDK de Qorvo, especificación UCI, datasheet, manual). Si no está documentado, decirlo explícitamente y proponer verificación con hardware real.
2. **No ejecutar acciones destructivas** (flasheo sobre firmware en producción, borrado de calibración, escritura de OTP) sin confirmación explícita e interactiva del usuario.
3. Mantener la coherencia de idioma: documentación y commits en español; identificadores de código en inglés (convención habitual del SDK de Qorvo).
4. Ante ambigüedad en un requisito, **preguntar antes de implementar**; no decidir unilateralmente.

---

*Este documento adapta al contexto de firmware UCI (C, SDK QM33) las convenciones de Git y documentación usadas en el proyecto hermano [i-mop-qorvo-cli-fw](https://github.com/EstebanMenti/i-mop-qorvo-cli-fw) (variante CLI del mismo hardware).*
