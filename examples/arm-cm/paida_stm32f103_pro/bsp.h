/*.$file${.::bsp.h} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*
* Model: dpp.qm
* File:  ${.::bsp.h}
*
* This code has been generated by QM 5.1.1 <www.state-machine.com/qm/>.
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*/
/*.$endhead${.::bsp.h} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
 #ifndef BSP_H
 #define BSP_H
 
 #define BSP_TICKS_PER_SEC    100U
 
 void BSP_init(void);
 void BSP_ledoff(void);
 void BSP_ledon(void);
 
 #endif /* BSP_H */

