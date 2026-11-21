# pr_tpp7 — Лабораторна робота №7, варіант 9 («Сортування масивів»)

Модернізована статична бібліотека з лабораторної роботи №4: обробка винятків, гарантії безпеки
винятків, RAII та модульне тестування (Google Test + CTest, залежності через vcpkg).

## Структура

```
include/   SortableArray.h, SortExceptions.h, SearchUtils.h, ScopeLogger.h
src/       SortableArray.cpp, SearchUtils.cpp          (статична бібліотека SortLib)
demo/      main.cpp                                    (демонстраційна програма)
antipattern/ main.cpp                                  (некоректні підходи до винятків)
tests/     SortTests.cpp                               (Google Test)
CMakeLists.txt, CMakePresets.json, vcpkg.json
```

## Клас SortableArray

Масив цілих чисел з фіксованою місткістю.

**Інваріанти:** `capacity_ > 0` і `items_.size() <= capacity_`. Внутрішній вектор є `private`,
тому некоректний стан через публічний інтерфейс недоступний. `at()` і `toVector()` повертають
значення або копію, а не посилання на внутрішні дані.

| Метод | Призначення | Винятки | Гарантія |
|---|---|---|---|
| `SortableArray(capacity)` | створення | `std::invalid_argument` (capacity = 0) | basic |
| `size()`, `capacity()` | властивості | — | no-throw |
| `at(i)` | доступ за індексом | `std::out_of_range` | no-throw змін стану |
| `minValue()`, `maxValue()`, `averageValue()` | екстремуми та середнє | `EmptyArrayException` | без змін стану |
| `add(v)` | додавання | `ArrayCapacityException` | **strong** |
| `replaceAll(vec)` | повна заміна | `ArrayCapacityException` | **strong** |
| `sortRange(first, last, method)` | сортування півінтервалу `[first, last)` | `InvalidSortMethodException`, `InvalidRangeException` | **strong** |

Коди `method`: `0` — сортування вставками, `1` — `std::sort`.

## Ієрархія винятків

```
std::runtime_error
└── SortException                 (базовий клас бібліотеки)
    ├── InvalidRangeException      first(), last(), size()
    ├── EmptyArrayException        operation()
    ├── ArrayCapacityException     capacity(), requested()
    └── InvalidSortMethodException code()
```

Стандартні винятки: `std::invalid_argument` (нульова місткість), `std::out_of_range` (індекс у `at()`
та `kthElement()`).

## Сценарії помилок (вимоги варіанта)

| # | Умова | Тип винятку |
|---|---|---|
| 1 | Нульова місткість конструктора | `std::invalid_argument` |
| 2 | Додавання до заповненого масиву / заміна більшим набором | `ArrayCapacityException` |
| 3 | Мін., макс., середнє порожнього масиву | `EmptyArrayException` |
| 4 | Порожній або неправильний діапазон `[first, last)` (`first >= last`, `last > size`) | `InvalidRangeException` |
| 5 | Некоректний код алгоритму сортування | `InvalidSortMethodException` |
| 6 | Індекс поза межами в `at()` | `std::out_of_range` |

## Гарантії безпеки

**Strong guarantee** у `sortRange()`: спочатку перевіряються параметри, потім сортується **копія**,
і лише після успіху `swap()` (без винятків) замінює внутрішній вектор. Якщо будь-який крок
кидає виняток, стан не змінюється.

**Strong guarantee** у `replaceAll()`: перевірка місткості → підготовка копії → `swap()`.

**Strong guarantee** у `add()`: перевірка місткості до `push_back()`.

**Деструктори** (`ScopeLogger`, стандартні контейнери) оголошені або є `noexcept`, тому
розкручування стека не викликає `std::terminate()`.

**Negative self-check (навмисна помилка):** у копії `replaceAll()` стан очищувався до перевірки
місткості. Тест `SortableArrayStrongGuaranteeTest.ReplaceAllPreservesStateOnException` впав
з `ASSERT_EQ(arr.size(), 2u)`, тобто помилка була виявлена. Після відновлення коду всі тести
проходять.

## Розкручування стека (ScopeLogger)

Очікувана послідовність, записана **до** запуску:

```
[ENTER] sortLevel1
[ENTER] sortLevel2
[ENTER] sortLevel3
Sorting range [3, 9) ...
[EXIT ] sortLevel3
[LOG] sortLevel2 caught: Invalid sort range: [3, 9) for size 6
[EXIT ] sortLevel2
[EXIT ] sortLevel1
[MAIN] Exception: Invalid sort range: [3, 9) for size 6
```

**Фактичний результат** (збірка GCC 13, Linux) збігся з очікуваним повністю. Для Visual Studio
перевірте вивід вручну: текст має бути той самий.

Висновок: деструктори викликаються у порядку, зворотному до конструювання (3 → 2 → 1).
Виняток переходить від нижнього рівня до `main()`. Рівень 2 перехоплює його через
`const SortException&`, виводить діагностику та повторно генерує через `throw;`, тому тип
`InvalidRangeException` зберігається.

## Антипатерни (`antipattern/main.cpp`)

- `throw 42;` і `throw std::string(...)` не належать до `std::exception`, тому обробник
  `catch (const std::exception&)` їх не перехоплює. Потрібен окремий `catch (int)` або `catch (...)`.
- `throw ex;` у `catch (const std::exception& ex)` створює новий об'єкт статичного типу
  `std::exception` (object slicing). Тип `std::invalid_argument` втрачається.
- `throw;` зберігає початковий тип винятку.

**Висновок:** винятки з класів, похідних від `std::exception`, дають єдиний інтерфейс `what()`,
можливість ієрархії та типізовану обробку. Примітивні типи і рядки не дають цього, тому для
бібліотеки їх використовувати не слід.

## Збирання та запуск

### Visual Studio 2022/2026 (vcpkg manifest)

1. Відкрийте каталог проєкту (Файл → Відкрити → Папка).
2. У терміналі Visual Studio в каталозі проєкту виконайте:
   ```
   vcpkg x-update-baseline --add-initial-baseline
   ```
   (якщо `vcpkg` не в PATH, використайте повний шлях до `vcpkg.exe`).
3. Перевірте шлях `CMAKE_TOOLCHAIN_FILE` у `CMakePresets.json` і виправте його під свою систему.
4. Оберіть пресет `x64-debug` → Збереження → Збирання (Ctrl+Shift+B).
5. Запуск тестів: Тест → Обозреватель тестов → Запустить все. Або запустіть `SortTests.exe` як
   цільову програму.
6. Демонстрація: запустіть `SortDemo.exe` і `AntipatternDemo.exe`.

### Командний рядок (CTest)

```
cmake --preset x64-debug
cmake --build out/build/x64-debug
ctest --test-dir out/build/x64-debug --output-on-failure
```

## Бібліотека з лабораторної №4 (SearchUtils.lib)

Файл `SearchUtils.lib` — це скомпільована x64 бібліотека MSVC. Вона експортує функції
`binarySearch`, `countOccurences`, `findFirst`, `findLast`, `kthElement`. Вихідний код з
`.lib` відновити неможливо, тому функції відтворено в `src/SearchUtils.cpp`, а сигнатури
підібрано за іменами. Щоб перевірити відповідність, виконайте в командному рядку
«Developer Command Prompt»:
```
dumpbin /exports SearchUtils.lib
```
