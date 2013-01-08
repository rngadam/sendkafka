# Pre-requisites

* need librdkafka installed in parent directory
 * https://github.com/edenhill/librdkafka
* need kafka broker running locally

# Building and installing

```
make
mv sendkafka /usr/local/bin
```

# Rsyslog configuration

/etc/rsyslog.conf configuration directive:

```
$ActionOMProgBinary /usr/local/bin/sendkafka
*.* :omprog:;RSYSLOG_SyslogProtocol23Format
```