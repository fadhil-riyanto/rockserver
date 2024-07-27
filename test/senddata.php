<?php

$fd = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if ($fd === false) {
        echo "err: " . socket_strerror(socket_last_error()) . "\n";
}

$result = socket_connect($fd, '127.0.0.1', 8998);

$sendbuf = "set0\r\n\r\nop1abc\r\n\r\nop2ghijk\r\n\r\n";
socket_send($fd, $sendbuf, strlen($sendbuf), 0);

socket_close($fd);
