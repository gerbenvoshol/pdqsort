 pdqsort.h - v1.0
 
 Public Domain QuickSORT -- public domain -- no warranty is offered or implied; use this code at your own risk

 This is a single header file with a public domain Quicksort implementation 

 ============================================================================
     
     Just include the file, like this:

            #include "pdqsort.h"

 ============================================================================

 Version History
        1.00  initial release

 This is the well-known sorting algorithm developed by C. A. R. Hoare. It is a comparison 
 sort and in this implementation, it is not a stable sort. Since the PDQSORT_SWAP is a macro
 it is possible to change it into a swap which can co-sort a dependent array (see example)

 LICENSE

 This software is dual-licensed to the public domain and under the following
 license: you are granted a perpetual, irrevocable license to copy, modify,
 publish, and distribute this file as you see fit.

 CREDITS
 
 Remi Dufour for the initial QuickSORT implementation (Public domain)

 Adapted by Gerben Voshol to macro style C include.
