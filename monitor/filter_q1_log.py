"""Save every monitor session without changing the displayed serial output."""

from datetime import datetime, timezone
from pathlib import Path

from platformio.device.monitor.filters.base import DeviceMonitorFilterBase


class Q1Log(DeviceMonitorFilterBase):
    NAME = "q1_log"

    def __call__(self):
        directory = Path(self.project_dir or Path.cwd()) / "logs" / "q1"
        directory.mkdir(parents=True, exist_ok=True)
        stamp = datetime.now(timezone.utc).strftime("%Y%m%dT%H%M%S.%fZ")
        role = self.environment or "unknown"
        path = directory / f"{role}-{stamp}.log"
        previous = getattr(self, "_log", None)
        if previous:
            previous.close()
        self._log = path.open("x", encoding="utf-8", newline="")
        print(f"--- Saving serial output to {path}")
        return self

    def rx(self, text):
        self._log.write(text)
        self._log.flush()
        return text

    def __del__(self):
        log = getattr(self, "_log", None)
        if log:
            log.close()
