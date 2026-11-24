# Container checks

Use the existing `hw2-lora` Docker image and `hw2-platformio` toolchain volume:

```sh
docker run --rm -v "$PWD":/work -v hw2-platformio:/root/.platformio -w /work hw2-lora sh -c 'pio run -e ap -e transmitter && cppcheck --check-level=exhaustive --enable=warning,style,performance,portability --error-exitcode=1 --suppress=missingIncludeSystem -I include -DA4_AP=1 src/main.cpp && cppcheck --check-level=exhaustive --enable=warning,style,performance,portability --error-exitcode=1 --suppress=missingIncludeSystem -I include -DA4_AP=0 src/main.cpp && g++ -std=c++11 -Wall -Wextra -Werror -I include test/transport_test.cpp -o /tmp/a4-test && /tmp/a4-test'
```

Use host PlatformIO for USB upload and serial monitoring. Do not flash a board
unless requested. Keep changes scoped to the requested checkpoint.
