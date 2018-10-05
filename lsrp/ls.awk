#!/bin/awk -f
{
  event = $1
  time = 0 + $2 # Make sure that "time" has a numeric type.
  node_id = $3
  pkt_size = 0 + $6
  level = $4

  if (level == "1" && event == "+" && $5 == "cbr") {
    sent++
    if (!startTime || (time < startTime)) {
      startTime = time

    }
  }

  if (level == "1" && event == "r" && $5 == "cbr") {
    receive++
    if (time > stopTime) {
      stopTime = time

    }
    recvdSize += pkt_size

  }
}

END {
  printf("start Time = %f, stopTime = %f\n", startTime, stopTime)
  printf("sent_packets\t %d\n",sent);
  printf("received_packets %d\n",receive);
  printf("PDR %.2f\n",((receive/sent)*100));
  printf("Received size\t %d\n",recvdSize);
  printf("Average Throughput[kbps] = %.2f\tStartTime=%.2f\tStopTime = %.2f\n", (recvdSize/(stopTime-startTime))*8/1000,startTime,stopTime);
}
