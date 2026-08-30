#!/usr/bin/env python3
import sys
import os
import time
import json
import argparse
from datetime import datetime

try:
    import serial
    import serial.tools.list_ports
except ImportError:
    serial = None

def find_esp32_port():
    if not serial:
        return None
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if "USB" in port.description or "ACM" in port.device or "UART" in port.description:
            return port.device
    return ports[0].device if ports else None

def main():
    parser = argparse.ArgumentParser(description="AutoGarage v2 Data Collector")
    parser.add_argument("--port", type=str, help="Serial port")
    parser.add_argument("--baud", type=int, default=115200, help="Baud rate")
    parser.add_argument("--output-dir", type=str, default="datasets", help="Output directory")
    args = parser.parse_args()

    port = args.port or find_esp32_port()
    if not port:
        print("Error: No serial port specified or found.")
        sys.exit(1)

    today_str = datetime.now().strftime("%Y-%m-%d")
    session_id = f"session_{int(time.time())}"
    session_dir = os.path.join(args.output_dir, today_str, session_id)
    os.makedirs(session_dir, exist_ok=True)

    raw_filepath = os.path.join(session_dir, "raw.jsonl")
    meta_filepath = os.path.join(session_dir, "metadata.json")

    metadata = {
        "session_id": session_id,
        "start_time": datetime.now().isoformat(),
        "port": port,
        "baud": args.baud,
        "firmware_version": "2.0.0"
    }

    with open(meta_filepath, "w", encoding="utf-8") as f:
        json.dump(metadata, f, indent=2)

    print(f"Connecting to {port} @ {args.baud} baud...")
    print(f"Logging session data to: {raw_filepath}")

    if not serial:
        print("pyserial is not installed.")
        return

    try:
        ser = serial.Serial(port, args.baud, timeout=1.0)
    except Exception as e:
        print(f"Failed to open serial port {port}: {e}")
        sys.exit(1)

    line_count = 0
    with open(raw_filepath, "a", encoding="utf-8") as out_file:
        try:
            while True:
                line = ser.readline().decode("utf-8", errors="ignore").strip()
                if not line:
                    continue
                if line.startswith("{") and line.endswith("}"):
                    out_file.write(line + "\n")
                    out_file.flush()
                    line_count += 1
        except KeyboardInterrupt:
            print(f"\nSession finished. Total events captured: {line_count}")

if __name__ == "__main__":
    main()
