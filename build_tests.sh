#!/bin/bash

set -e

# Сборка программы
make tests

# запуск программы
./tests

echo "Ypa! Cработало"
