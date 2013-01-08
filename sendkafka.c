/*
 * librdkafka - Apache Kafka C library
 *
 * Copyright (c) 2012, Magnus Edenhill
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * using the Kafka driver from librdkafka
 * (https://github.com/edenhill/librdkafka)
 */

#include <ctype.h>
#include <signal.h>
#include <string.h>

/* Typical include path would be <librdkafka/rdkafkah>, but this program
 * is builtin from within the librdkafka source tree and thus differs. */
#include "librdkafka/rdkafka.h"  /* for Kafka driver */


static int run = 1;

static void stop (int sig) {
	run = 0;
}

int main (int argc, char **argv) {
	rd_kafka_t *rk;
	char *broker = NULL;
	char *topic = NULL;
	int partition = 0;

	topic = "syslog";
	partition = 0;

	signal(SIGINT, stop);

	/*
	 * Producer
	 */
	char buf[2048];
	int sendcnt = 0;

	/* Create Kafka handle */
	if (!(rk = rd_kafka_new(RD_KAFKA_PRODUCER, broker, NULL))) {
		perror("kafka_new producer");
		exit(1);
	}

	fprintf(stderr, "%% Type stuff and hit enter to send\n");
	while (run && (fgets(buf, sizeof(buf), stdin))) {
		int len = strlen(buf);
		char *opbuf = malloc(len + 1);
		strncpy(opbuf, buf, len + 1);

		/* Send/Produce message. */
		rd_kafka_produce(rk, topic, partition, RD_KAFKA_OP_F_FREE, opbuf, len);
		fprintf(stderr, "%% Sent %i bytes to topic "
			"%s partition %i\n", len, topic, partition);
		sendcnt++;
	}

	/* Wait for messaging to finish. */
	while (rd_kafka_outq_len(rk) > 0)
		usleep(50000);

	/* Since there is no ack for produce messages in 0.7
	 * we wait some more for any packets to be sent.
	 * This is fixed in protocol version 0.8 */
	if (sendcnt > 0)
		usleep(500000);

	/* Destroy the handle */
	rd_kafka_destroy(rk);

	return 0;
}
