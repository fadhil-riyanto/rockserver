<?php

$fdptr = fopen("shared_data", "rb");
$fullbin = fread($fdptr, filesize("shared_data"));

$unpacked = unpack("I2", $fullbin, 0);

var_dump($unpacked);