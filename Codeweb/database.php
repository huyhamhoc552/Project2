<?php
class Database {
    private static $dbName = 'LoRa';
    private static $dbHost = 'localhost';
    private static $dbUsername = 'root';
    private static $dbUserpassword = '';
    private static $cont = null;

    public function __construct() {
        die();
    }

    public static function connect() {
        if (null == self::$cont) {
            try {
                self::$cont = new PDO("mysql:host=".self::$dbHost.";dbname=".self::$dbName, self::$dbUsername, self::$dbUserpassword);
            } catch (PDOException $e) {
                die($e->getMessage());
            }
        }
        return self::$cont;
    }

    public static function disconnect() {
        self::$cont = null;
    }
}
?>