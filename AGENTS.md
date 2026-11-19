# Container checks

Use the existing `hw2-lora` Docker image and `hw2-platformio` toolchain volume:

```sh
docker run --rm -v "$PWD":/work -v hw2-platformio:/root/.platformio -w /work hw2-lora sh -c 'pio run && cppcheck --enable=warning,style,performance,portability --error-exitcode=1 --suppress=missingIncludeSystem src/main.cpp'
```

Use host PlatformIO for USB upload and serial monitoring. Do not flash a board
unless requested. Keep changes scoped to the requested checkpoint.
