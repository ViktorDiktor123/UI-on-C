import json

# Обратите внимание: я добавил пропущенную букву 'J' в ваш алфавит
char = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890()!?.,;:-+*/ '
font = {}
print(len(char))

print("Инструкция: вводите по 3 символа на строку (например, '# #' или ' # '). Всего 5 строк на букву.")

for a in char:
    print(f"\n--- Рисуем символ '{a}' ---")
    c = []
    for i in range(5):
        # Запрашиваем строку. Если ввели меньше 3 символов, дополняем пробелами
        line = input(f"Строка {i+1}: ").ljust(3)
        c.append(line)
        
    s = ''
    for b in range(5):
        n = 0  # ИСПРАВЛЕНО: обнуляем счетчик для КАЖДОЙ строки!
        for i in range(2, -1, -1):
            if i == 2: x = 1
            elif i == 1: x = 2
            elif i == 0: x = 4
            
            # Проверяем, что там не пробел (например, '#' или '1')
            if c[b][i] != ' ': 
                n += x
                
        # ИСПРАВЛЕНО: переводим код в формат \u000X, чтобы Си-парсер не ломался
        s += f"\\u{n:04x}"
        
    font[a] = s

# Обернем в структуру, которую мы закладывали в Си
output_data = {
    "width": 3,
    "height": 5,
    "glyphs": font
}

# ИСПРАВЛЕНО: wrire -> write, а также убираем двойное экранирование слэшей
json_raw = json.dumps(output_data, indent=2)
json_raw = json_raw.replace("\\\\u", "\\u")

with open('font.json', 'w', encoding='utf-8') as f:
    f.write(json_raw) # ИСПРАВЛЕНО: write

print("\n=== ШРИФТ УСПЕШНО СОХРАНЕН В 'font.json'! ===")
