#!/usr/bin/env python3
"""Build Cat-Shell and check that both help flags print the command list."""

import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def main() -> int:
    sources = sorted((ROOT / "src").glob("*.cpp"))
    if not sources:
        print("no C++ sources found", file=sys.stderr)
        return 1

    with tempfile.TemporaryDirectory() as tmp:
        exe = Path(tmp) / ("cat_shell.exe" if sys.platform == "win32" else "cat_shell")
        subprocess.run(
            ["g++", "-std=c++17", "-Wall", "-Wextra", *(str(p) for p in sources), "-o", str(exe)],
            check=True,
        )

        for flag in ("--help", "-h"):
            result = subprocess.run(
                [str(exe), flag],
                capture_output=True,
                text=True,
                encoding="utf-8",
                errors="replace",
                check=True,
                timeout=10,
            )
            for expected in ("help", "version", "kitty feed", "echo [text]", "mkdir [name]"):
                if expected not in result.stdout:
                    print(f"{flag}: missing {expected!r} in help output", file=sys.stderr)
                    return 1

    print("smoke test passed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
