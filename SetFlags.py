Import("env")
Import("projenv")

# Простой вывод в консоль, чтобы убедиться, что скрипт запускается
# print("SetFlags.py is running!")

# Применяем флаги
for e in [env, projenv, DefaultEnvironment()]:
    e.Append(
        CCFLAGS=[
            "-mfloat-abi=hard",
            "-mfpu=fpv4-sp-d16",
        ],
        LINKFLAGS=[
            "-mfloat-abi=hard",
            "-mfpu=fpv4-sp-d16",
        ]
    )
    # Выводим подтверждение, что флаги добавлены
    # print("Flags added to environment:", e.get("CCFLAGS"))
    # print("Flags added to environment:", e.get("LINKFLAGS"))