#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>

#include "main.h"
#include "ina260.h"
#include "raspberryPiSensors.h"

static int keep_running;

RETSIGTYPE stop_server(int a) {
    keep_running = 0;
}


int main(){

    gpmuInit();

    // Print log errors to stderr
    snmp_enable_stderrlog();

    // Make us a agentx client
    netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_ROLE, 1);

    // Initialize TCP/IP 
    SOCK_STARTUP;

    // Initialize the agent library
    init_agent("gpmu-daemon");


    // MIB Code created by mib2c
    init_raspberryPiSensors();

    // gpmu-daemon will be used to read gpmu-daemon.conf
    init_snmp("gpmu-daemon");

    // In case the kill command comes through
    keep_running = 1;
    signal(SIGTERM, stop_server);
    signal(SIGINT, stop_server);

    snmp_log(LOG_INFO, "gmpu-daemon is up and running.\n");

    //  Main wwhile loop
    while (keep_running){


        agent_check_and_process(1);
    }

    // Shut things down
    snmp_shutdown("gpmu-daemon");
    SOCK_CLEANUP;

    return 0;
}
