<?php

$fd = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if ($fd === false) {
        echo "err: " . socket_strerror(socket_last_error()) . "\n";
}

$result = socket_connect($fd, '127.0.0.1', 8998);

/* test send endfile */
$sendbuf = "abcpiabcpia\r\n\r\nbcpiabcpi";
// for($i = 0; $i < 100; $i++) {
//         socket_send($fd, $sendbuf, strlen($sendbuf), 0);
// }

socket_send($fd, $sendbuf, strlen($sendbuf), 0);

socket_close($fd);
