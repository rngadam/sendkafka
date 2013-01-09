# Pre-requisites
* sudo apt-get install zlib1g-dev
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
$ModLoad omprog
$ActionOMProgBinary /usr/local/bin/sendkafka
*.* :omprog:;RSYSLOG_SyslogProtocol23Format
```

Check configuration

```
rsyslogd -f /etc/rsyslog.conf -N1
```

Modify AppArmor:

```
/etc/apparmor.d/local/usr.sbin.rsyslogd
```

Reload

If you're having problems, look into AppArmor or SELinux CAP.