# Queue
## Выпускное задание
На одном из занятий мы изучали пул потоков. Ключевым элементом этого шаблона была общая очередь, которая была доступна всем потокам. Потоки можно поделить на две группы - первая группа, которая могла писать в очередь, формируя таким образом задания для остальных, и вторая группа потоков, которая могла читать из этой очереди. В таком виде наше решение могло быть использовано только на одной машине. Наша задача позволить распределить потоки работающие с этой очередью на разные машины.

Можно сразу ввести новые понятия - то что создает задание в очереди, фактически пишет в очередь, называется продюсер, то что читает из очереди называется консьюмер. Фактически наша задача сводится к написанию сервера который будет обеспечивать по сети доступ к методам  `push_back()` для продюсеров и `front() / pop()` для консьюмеров класса `std::queue`.

Поскольку разрабатывая внешний сервер полагаться на его бесперебойную работу мы не можем, нам необходимо иметь подстраховку в виде копии нашей очереди на диске. На первом этапе мы можем ограничиться очередью только в памяти, но всегда должны помнить, что должна быть альтернатива в виде дисковой копии.

Отдельностоящий сервер должен будет взаимодействовать с другими машинами для подключений продюсеров и консьюмеров. Для этого он будет сопровождаться клиентской библиотекой, которая будет брать на себя сетевое взаимодействие с сервером и иметь несколько основных методов, например подключение к серверу с выбором экземпляра очереди по имени, аналог метода push_back для продюсера, аналог метода pop для консьюмера, который может работать в синхронном блокирующем режиме так и в асинхронном, ну и конечно же метод завершения работы.

Что такое клиентская библиотека? Клиентская библиотека - прежде всего код с нужными классами и фунциями, а уж есть там .so или .a уже не имеет значения. Грубо говоря, если мне хочется поработать с вашей очередью я должен иметь понятный сценарий ее подключения, например, должен написать `#include <mgafka>` и слинковаться с `mgafka`, для подключения к серверу - `auto h = MgafkaConnection(“localhost”, 6519);` и далее по API.

В одну и ту же очередь допускается запись нескольких разных продюсеров. Из одной и той же очереди могут читать разные консьюмеры. Очередь на сервере никогда не очищается. Исключением будет ситуация, когда заканчивается оперативная или дисковая память. В этой ситуации должны быть удалены самые старые записи, которые хранятся в очереди.

За хранение информации о последней обработанной записи отвечает клиентская сторона. При подключении к серверу должна быть передана специальная метка, по которой сервер поймёт начиная с какой позиции в очереди необходимо отдавать данные. С каждой возвращенной порцией данных сервер обязан вернуть новую метку, клиент обязан эту метку сохранить.

Поскольку нет возможности работать с типизированными данными в качестве типа хранения мы выбираем строку.

Когда и кем создаются очереди - можно рассмотреть вариант с автоматическим созданием в момент первого подключения консьюмера или продьюсера.

Очередь не очищается (мы сделаем нечто типа кафки, справедливо не для всех очередей), у нас есть физический ресурс диска, когда он будет подходить к концу, можно удалить самые старые данные, вне зависимости от того - обработали их или еще нет.

Критерий отсечения должен быть понятный - на диске он может быть примитивным - держит заполненным какой-то объем на диске, при превышении удаляем старое. Держать в памяти всю очередь не обязательно, достаточно именть бегущее окно - самый частый сценарий будет чтение свежих записей. Окна можно подгружать по мере необходимости, если подключится клиент, запросивший совсем старые данные.

Уметь восстанавливать данные после рестарта/падения и прочих причин крайне полезно, память не обязательно заполнять очередью полностью.

На диск писать можно сразу по получению данных, стараться делать это либо асинхронно либо отдельным потоком, чтобы скорость записи на диск не тормозила клиентов, скорее всего для этого потребуется внутренняя очередь на запись.

Функционал на получение списка очередей как и всех остальных функций можно расширять на свое усмотрение, часть из них совершенно точно могут помочь в диагностике и администрировании будущего сервера.
