import requests

# Адрес сервера (предполагается, что C++ сервер запущен на localhost:8080)
url = 'http://localhost:8080/find_polynomials'

# Данные для запроса (m и t фиксированные)
payload = {
    "m_start": 3,
    "m_end": 4,
    "t_start": 3,
    "t_end": 3,
    "mode": "gpu"
}

# Отправка POST-запроса
response = requests.post(url, json=payload)

# Обработка результата
if response.status_code == 200:
    data = response.json()
    print(data)
    print("Полученные полиномы:")
    for item in data["results"]:
        print(f"m = {item['m']}, t = {item['t']}, polynom = {item['polynom']}")
else:
    print(f"Ошибка: {response.status_code}")
    print(response.text)