# NUMA-aware-DSU

Внутри используется `libcds` и для сборки следует передать путь до места, где он лежит:
`-DCMAKE_PREFIX_PATH=/path/.../`


###Benchmark
Запускает benchmark. На данный момент как результат отдает два числа -- время в секундах запуска numa-алгоритма и обычного.

Можно запускать на случайном графе или передвать файл.
* Для случайного графа: `./benchmark random N E [THREADS]`
* Для конкретного файла `./benchmark filename [THREADS]`

Файл с графом должен быть в специальном формате. Сначала должны быть записаны число вершин и число ребер в графе, а затем перечислены ребра как пары выршин.