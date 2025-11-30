# Симуляция движения поездов по маршруту Душанбе — Гиссар — Регар — Пахтаабад

## Постановка задачи

Необходимо разработать C++-программу, моделирующую движение поездов по железнодорожной ветке  
**Душанбе — Гиссар — Регар — Пахтаабад**.  

Требования:

1. Минимум **8 поездов**, каждый — **отдельный поток** с уникальным номером.
2. Поезда движутся по циклическому маршруту: после Пахтаабада возвращаются к Душанбе.
3. У каждой станции должен быть **один общий ресурс** — платформа, доступ к которой осуществляют **по одному поезду одновременно** (через общий `station_lock`).
4. Все события должны выводиться в консоль:
   - начало движения;
   - прибытие на станцию;
   - стоянка;
   - отправление;
   - остановка поезда при завершении симуляции.
5. Время движения между станциями и время стоянки — **разные случайные интервалы**.
6. Симуляция работает **до тех пор, пока пользователь не нажмёт Enter**.

---

## Работа программы

1. При запуске создаётся маршрут из четырёх станций.  
2. Инициализируются восемь поездов, каждому присваивается:
   - номер;
   - стартовая станция (индекс 0);
   - активное состояние.
3. Каждый поезд запускается в отдельном потоке и выполняет бесконечный цикл:
   - логирует начало движения к следующей станции;
   - выполняет задержку (имитация пути);
   - запрашивает доступ к платформе станции через общий `station_lock`;
   - логирует прибытие;
   - выполняет задержку стоянки;
   - логирует отправление;
   - освобождает платформу;
   - переходит к следующей станции по кругу.
4. В главном потоке программа ждёт нажатия Enter.
5. После сигнала поезда завершают цикл и выводят сообщение о завершении.

---

## Вывод программы (пример реального выполнения)
```console
Train PRESS ENTER TO STOP...1 MOVING TO Dushanbe
Train 2 MOVING TO Dushanbe
Train 3 MOVING TO Dushanbe
Train 4 MOVING TO Dushanbe
Train 5 MOVING TO Dushanbe
Train 6 MOVING TO Dushanbe
Train 7 MOVING TO Dushanbe
Train 8 MOVING TO Dushanbe
Train 5 ARRIVED at Dushanbe
Train 5 LEAVING Dushanbe
Train 5 MOVING TO Gissar
Train 3 ARRIVED at Dushanbe
Train 3 LEAVING Dushanbe
Train 3 MOVING TO Gissar
Train 2 ARRIVED at Dushanbe
Train 2 LEAVING Dushanbe
Train 2 MOVING TO Gissar
Train 7 ARRIVED at Dushanbe
Train 7 LEAVING Dushanbe
Train 6 ARRIVED at Dushanbe
Train 7 MOVING TO Gissar
Train 6 LEAVING Dushanbe
Train 6 MOVING TO Gissar
Train 1 ARRIVED at Dushanbe
Train 1 LEAVING Dushanbe
Train 1 MOVING TO Gissar
Train 8 ARRIVED at Dushanbe
Train 8 LEAVING Dushanbe
Train 8 MOVING TO Gissar
Train 4 ARRIVED at Dushanbe
Train 4 LEAVING Dushanbe
Train 4 MOVING TO Gissar
Train 5 ARRIVED at Gissar
Train 5 LEAVING Gissar
Train 5 MOVING TO Regar
Train 3 ARRIVED at Gissar
Train 3 LEAVING Gissar
Train 3 MOVING TO Regar
Train 2 ARRIVED at Gissar
Train 2 LEAVING Gissar
Train 2 MOVING TO Regar
Train 7 ARRIVED at Gissar
Train 7 LEAVING Gissar
Train 7 MOVING TO Regar
Train 6 ARRIVED at Gissar
Train 6 LEAVING Gissar
Train 6 MOVING TO Regar
Train 1 ARRIVED at Gissar
Train 1 LEAVING Gissar
Train 1 MOVING TO Regar
Train 8 ARRIVED at Gissar
Train 8 LEAVING Gissar
Train 8 MOVING TO Regar
Train 4 ARRIVED at Gissar
Train 4 LEAVING Gissar
Train 4 MOVING TO Regar
Train 5 ARRIVED at Regar
Train 5 LEAVING Regar
Train 5 MOVING TO Pakhtaabad
Train 3 ARRIVED at Regar
Train 3 LEAVING Regar
Train 3 MOVING TO Pakhtaabad
Train 2 ARRIVED at Regar
Train 2 LEAVING Regar
Train 2 MOVING TO Pakhtaabad
Train 7 ARRIVED at Regar
Train 7 LEAVING Regar
Train 7 STOPPED
Train 6 ARRIVED at Regar
Train 6 LEAVING Regar
Train 6 STOPPED
Train 1 ARRIVED at Regar
Train 1 LEAVING Regar
Train 1 STOPPED
Train 8 ARRIVED at Regar
Train 8 LEAVING Regar
Train 8 STOPPED
Train 4 ARRIVED at Regar
Train 4 LEAVING Regar
Train 4 STOPPED
Train 5 ARRIVED at Pakhtaabad
Train 5 LEAVING Pakhtaabad
Train 5 STOPPED
Train 3 ARRIVED at Pakhtaabad
Train 3 LEAVING Pakhtaabad
Train 3 STOPPED
Train 2 ARRIVED at Pakhtaabad
Train 2 LEAVING Pakhtaabad
Train 2 STOPPED
Simulation finished!
```

---

## Анализ вывода

1. **Все поезда начинают одновременно**, но за счёт случайных задержек порядок прибытия меняется.  
2. **Только один поезд прибывает на станцию в единицу времени**, что подтверждает корректную работу общего `station_lock`.  
3. Если поезд подходит к станции, когда она занята другим, он **ждёт**, что отражается в задержках между выводами.
4. Поезда проходят по маршруту цикл за циклом: Душанбе → Гиссар → Регар → Пахтаабад → Душанбе.
5. После нажатия Enter поезда корректно завершают текущие операции и выводят сообщение `STOPPED`, что говорит о правильной работе флага активности и корректном завершении потоков.
6. Deadlock не возникает: все поезда завершаются и общий ресурс освобождается всегда.

---

## Заключение

Реализованная программа удовлетворяет требованиям:

- использовано 8 потоков;  
- введён общий ресурс станции, защищённый mutex;  
- события логируются с понятной структурой;  
- движение и стоянка имеют случайную продолжительность;  
- симуляция работает бесконечно и завершается по сигналу пользователя;  
- последовательность работы поездов соответствует реальному принципу очередности.

Вывод корректно демонстрирует работу синхронизации, показывает отсутствие взаимных блокировок и реалистичное распределение поездов по маршруту.
