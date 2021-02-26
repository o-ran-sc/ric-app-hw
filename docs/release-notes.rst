.. This work is licensed under a Creative Commons Attribution 4.0 International License.
.. SPDX-License-Identifier: CC-BY-4.0
.. Copyright (C) 2020 AT&T


Release Notes
=============


This document provides the release notes for the Bronze Release of the HelloWorld (HW) xAPP.

.. contents::
   :depth: 3
   :local:


Version history
---------------

+--------------------+--------------------+--------------------+--------------------+
| **Date**           | **Ver.**           | **Author**         | **Comment**        |
|                    |                    |                    |                    |
+--------------------+--------------------+--------------------+--------------------+
| 2021-02-26         | 1.1.1              |   Shraboni Jana    | Eighth draft       |
|                    |                    |                    |                    |
+--------------------+--------------------+--------------------+--------------------+



Summary
-------
The HelloWorld xApp Dawn release has a full implementation of the HW E2 SM including subscription, indication and control as well as C++ class wrappers for all the E2 messages related to HW E2 SM.
The HW xAPP demonstrates E2 and A1 interface interactions, persistent storage read-write, RMR and A1 healthcheck. 
HW xAPP uses its HelloWorld E2SM (can be found at hw/docs/).


Release Data
------------

+--------------------------------------+--------------------------------------+
| **Project**                          | RAN Intelligent Controller           |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Repo/commit-ID**                   |        ric-app/hw                    |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Release designation**              |              Dawn                    |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Release date**                     |      2021-02-26                      |
|                                      |                                      |
+--------------------------------------+--------------------------------------+
| **Purpose of the delivery**          | open-source Hello World xAPP         |
|                                      |                                      |
|                                      |                                      |
+--------------------------------------+--------------------------------------+

Components
----------

- *src/* contains the main source code. Under that directory :
  
  + *xapp.hpp, xapp.cc* is generic class which implements all the xAPP functionalities for xapp developer to pick and choose.
  + *xapp-utils/* contains generic classes for - persistent data management, configuration management, RMR send/receive etc.
  + *xapp-asn/* contains generic classes for generating/processing ASN1  E2AP and E2SM messages.
  + *xapp-mgmt/* contains code specific xapp management of subscriptions and received messages.

- *test/* contains unit tests showing how to use various components.

- *defs_hwe2sm/* contains helloworld e2sm asn1c definitions  
    

Limitations
-----------
- The HW xAPP doesn't have any usecase in particular to display SDL capabilities and hence incorporated only in unit tests.

- The subscription process assumes, on sending subscription request results in valid subscription response. 

- The HW xAPP doesn't address any RIC usecase in particular.
