import requests

url = 'http://localhost:8080/find_polynomials'

payload = {
    "m_start": 3,
    "m_end": 4,
    "t_start": 3,
    "t_end": 3,
    "mode": "cpu"  # или "cpu"
}

response = requests.post(url, json=payload)

if response.status_code == 200:
    data = response.json()
    print("Полученные результаты:")
    for item in data.get("results", []):
        m = item["m"]
        t = item["t"]
        polynom = item.get("polynom")
        iterations = item.get("iterations", "N/A")
        time_sec = item.get("time_seconds", "N/A")
        error = item.get("error")

        if polynom is not None:
            print(f"[OK] m={m}, t={t} → polynom: {polynom}, iterations: {iterations}, time: {time_sec:.4f}s")
        else:
            print(f"[ERR] m={m}, t={t} → ошибка: {error}, iterations: {iterations}, time: {time_sec:.4f}s")
else:
    print(f"Ошибка HTTP: {response.status_code}")
    print(response.text)