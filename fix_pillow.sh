#!/bin/bash
# Run this on your Raspberry Pi to fix the Pillow/ImageTk error
echo "Fixing Pillow install..."

# Install system tk dev libraries (required for ImageTk to build)
sudo apt-get install -y \
    python3-tk \
    tk-dev \
    libtk8.6 \
    python3-pil.imagetk

# Force reinstall Pillow inside the venv WITHOUT system-site-packages fallback
# so it compiles its own ImageTk support against the tk-dev headers
~/robot_venv/bin/pip uninstall -y Pillow 2>/dev/null || true
~/robot_venv/bin/pip install --no-cache-dir --force-reinstall Pillow

# Quick test
~/robot_venv/bin/python3 -c "from PIL import ImageTk; print('✓ ImageTk OK')" \
  && echo "All good! Run: bash ~/run_robot.sh" \
  || echo "Still broken — trying fallback..."

# Fallback: use the system PIL directly (symlink into venv)
if ! ~/robot_venv/bin/python3 -c "from PIL import ImageTk" 2>/dev/null; then
    echo "Using system PIL fallback..."
    VENV_SITE=$(~/robot_venv/bin/python3 -c "import site; print(site.getsitepackages()[0])")
    SYS_PIL=$(python3 -c "import PIL; import os; print(os.path.dirname(PIL.__file__))")
    ln -sfn "$SYS_PIL" "$VENV_SITE/PIL"
    ~/robot_venv/bin/python3 -c "from PIL import ImageTk; print('✓ ImageTk OK via fallback')"
fi
