<?php
define("U_INT16_MAX", 65535);

class utils {
        public static function openssl_genrandstr($length = 10) {
                return bin2hex(openssl_random_pseudo_bytes($length));
        }
            
}

class huge_memmap 
{

}

class generate_test {
        private $fd;
        private $cur_rand_key = null;
        private $buf = null;
        public function __construct()
        {
                $fd = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
                if ($fd === false) {
                        echo "err: " . socket_strerror(socket_last_error()) . "\n";
                }

                $this->fd = $fd;
                $this->mk_sockconn("127.0.0.1", 8998);
        }

        private function mk_sockconn($host, $port)
        {
                socket_connect($this->fd, $host, $port);
        }

        public function generate_char_n($char, $n)
        {
                $genbuf = NULL;
                for($i = 0; $i < $n; $i++) {
                        $genbuf = $genbuf . $char;
                }

                return $genbuf;
        }

        public function send($opcode, $buf)
        {
                /* generate random string that used as key, then return $this in order to verify the data */
                $randstr = utils::openssl_genrandstr();

                var_dump(["generated" => $randstr]);
                $formatted = sprintf("%s \"%s\" \"%s\"\r\n\r\n", $opcode, $randstr, $buf);
                socket_send($this->fd, $formatted, strlen($formatted), 0);
                // var_dump($formatted);
        }

        public function verify_data() : bool 
        {
                $buf = NULL;
                socket_recv($this->fd, $buf, U_INT16_MAX, MSG_WAITALL);
                printf($buf);
        }

        public function __destruct()
        {
                sleep(4);
                socket_close($this->fd);
        }
}
$tmpbuf = null;

$gentest = new generate_test();
for($i = 0; $i < 1; $i++) {
        // $tmpbuf = $tmpbuf . $gentest->generate_char_n("a", 20);
        $gentest->send("set", $gentest->generate_char_n("a", 50000));
}return -3;

print(strlen($tmpbuf));