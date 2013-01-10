# Pre-requisites

* sudo apt-get install zlib1g-dev
* need librdkafka installed in parent directory
 * https://github.com/edenhill/librdkafka
* need kafka broker running locally

# rsyslog v7

add Ubuntu repository for rsyslog (precise)

edit /etc/apt/sources.d/adiscon

```
deb http://ubuntu.adiscon.com/v7-devel precise/
deb-src http://ubuntu.adiscon.com/v7-devel precise/
```

...and then... (make sure it installs v7)

```
apt-get install rsyslog
```

# Building and installing sendkafka

```
make
mv sendkafka /usr/local/bin
```

# Rsyslog configuration

/etc/rsyslog.conf configuration directive:

At the start:

```
$ModLoad omprog
```
In the global section:

```
$template JSONDefault, "{\"message\":\"%msg:::json%\",\"fromhost\":\"%HOSTNAME:::json%\",\"facility\":\"%syslogfacility-text%\",\"priority\":\"%syslogpriority-text%\",\"timereported\":\"%timereported:::date-rfc3339%\",\"timegenerated\":\"%timegenerated:::date-rfc3339%\"}\n",json

```
After the very last line:

```
$ActionOMProgBinary /usr/local/bin/sendkafka
*.* :omprog:;cee_enhanced
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