# SELinux_Windows
A portage of SELinux for Windows 7

An implementation of SELinux using Type Enforcement to protect Windows 7.
Based on SSDT hooking
Use reg ex to label the file system
All contexts are dynamically computed


https://www.sstic.org/media/SSTIC2012/SSTIC-actes/contrle_dacces_mandataire_pour_windows_7/SSTIC2012-Article-contrle_dacces_mandataire_pour_windows_7-toinard_gros_briffaut_2.pdf


Cet article présente une implémentation novatrice pour le renforcement des fonctions de contrôle d'accès de Windows 7. Basée sur le Type Enforcement, cette approche propose de mettre en place un contrôle d'accès de type mandataire supportant l'application de propriétés de sécurité avancées.
De façon similaire à LSM pour le noyau Linux, notre implémentation offre un contrôle d'accès à grain reposant sur le détournement des appels système. L'ajout d'une labellisation du système de fichiers offre une correspondance précise entre la ressource et son contexte de sécurité. De plus, dans le but de faciliter l'écriture d'une politique de sécurité, nous avons mis en place un mécanisme d'apprentissage qui se base sur les événements observés par notre solution pour les transformer en règles de contrôle d'accès.

Ainsi, cette implémentation assure le contrôle des flux d'information directs, que ce soit entre un sujet et un objet ou entre deux sujets, ainsi protection ou confinement contre les attaques de type 0-day. Cet article propose également une preuve de concept développée pour Windows 7, portable sur les autres systèmes d'exploitation de la famille Windows. Nous avons pu tester l'impact de notre implémentation sur les performances du système. 


This paper presents a novel solution of Mandatory Access Control (mac) providing Type Enforcement for the Windows 7 kernel. Our approach enforces a fine-grained mac policy to control the system call permissions. In contrast with the Windows Mandatory Integrity Control, access control is supported. Labeling of the system resources provides security contexts with consistent types. An automated policy generation process facilitates the definition of the required rules for the various applications. The various system calls are controlled through an extensible kernel hooking framework. Thus, an efficient method is available for controlling direct information flows and preventing against 0-day vulnerabilities. Despite the purpose is to provide a proof of concept, the performance of the proposed solution is encouraging us to follow this approach. The current overhead can be further reduced using a compiled policy method. Moreover, that solution implements a control mechanism that can guarantee advanced security properties associated with transitive information flows.
