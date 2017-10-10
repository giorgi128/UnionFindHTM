C
C  This file contains defines required by the PAPI Fortran interface.
C  It is automagically generated by genpapifdef.c
C  DO NOT modify its contents and expect the changes to stick.
C  Changes MUST be made in genpapifdef.c instead.
C


C
C  General purpose defines.
C

#define PAPI_NULL          -1
#define PAPI_VER_CURRENT   84148224
#define PAPI_VERSION       84148480
#define PAPI_MAX_PRESET_EVENTS 128
#define PAPI_NOT_INITED    0
#define PAPI_LOW_LEVEL_INITED 1
#define PAPI_HIGH_LEVEL_INITED 2
#define PAPI_THREAD_LEVEL_INITED 4
#define PAPI_DOM_USER      1
#define PAPI_DOM_KERNEL    2
#define PAPI_DOM_OTHER     4
#define PAPI_DOM_SUPERVISOR 8
#define PAPI_DOM_ALL       15
#define PAPI_DOM_MIN       1
#define PAPI_DOM_MAX       15
#define PAPI_DOM_HWSPEC    ((-2147483647) - 1)
#define PAPI_STOPPED       1
#define PAPI_RUNNING       2
#define PAPI_PAUSED        4
#define PAPI_NOT_INIT      8
#define PAPI_OVERFLOWING   16
#define PAPI_PROFILING     32
#define PAPI_MULTIPLEXING  64
#define PAPI_ATTACHED      128
#define PAPI_CPU_ATTACHED  256
#define PAPI_QUIET         0
#define PAPI_VERB_ECONT    1
#define PAPI_VERB_ESTOP    2
#define PAPI_MIN_STR_LEN   64
#define PAPI_HUGE_STR_LEN  1024
#define PAPI_MAX_STR_LEN   128
#define PAPI_NUM_ERRORS    25
#define PAPI_MULTIPLEX_DEFAULT 0
#define PAPI_MULTIPLEX_FORCE_SW 1
#define PAPI_DEBUG         2
#define PAPI_MULTIPLEX     3
#define PAPI_DEFDOM        4
#define PAPI_DOMAIN        5
#define PAPI_DEFGRN        6
#define PAPI_GRANUL        7
#define PAPI_DEF_MPX_NS    8
#define PAPI_MAX_MPX_CTRS  11
#define PAPI_PROFIL        12
#define PAPI_PRELOAD       13
#define PAPI_CLOCKRATE     14
#define PAPI_MAX_HWCTRS    15
#define PAPI_HWINFO        16
#define PAPI_EXEINFO       17
#define PAPI_MAX_CPUS      18
#define PAPI_ATTACH        19
#define PAPI_SHLIBINFO     20
#define PAPI_LIB_VERSION   21
#define PAPI_COMPONENTINFO 22
#define PAPI_DETACH        1
#define PAPI_GRN_THR       1
#define PAPI_GRN_MIN       1
#define PAPI_GRN_PROC      2
#define PAPI_GRN_PROCG     4
#define PAPI_GRN_SYS       8
#define PAPI_GRN_SYS_CPU   16
#define PAPI_GRN_MAX       16
#define PAPI_DERIVED       1
#define PAPI_PROFIL_POSIX  0
#define PAPI_PROFIL_RANDOM 1
#define PAPI_PROFIL_WEIGHTED 2
#define PAPI_PROFIL_COMPRESS 4
#define PAPI_PROFIL_BUCKET_16 8
#define PAPI_PROFIL_BUCKET_32 16
#define PAPI_PROFIL_BUCKET_64 32
#define PAPI_USR1_LOCK     0
#define PAPI_USR2_LOCK     1
#define PAPI_LOCK_USR1     0
#define PAPI_LOCK_USR2     1
#define PAPI_LOCK_NUM      2
#define PAPI_MH_MAX_LEVELS 6
#define PAPI_USR1_TLS      0
#define PAPI_USR2_TLS      1
#define PAPI_TLS_USR1      0
#define PAPI_TLS_USR2      1
#define PAPI_TLS_HIGH_LEVEL 2
#define PAPI_TLS_NUM       3
#define PAPIF_DMEM_VMSIZE  2
#define PAPIF_DMEM_RESIDENT 3
#define PAPIF_DMEM_HIGH_WATER 4
#define PAPIF_DMEM_SHARED  5
#define PAPIF_DMEM_TEXT    6
#define PAPIF_DMEM_LIBRARY 7
#define PAPIF_DMEM_HEAP    8
#define PAPIF_DMEM_LOCKED  9
#define PAPIF_DMEM_STACK   10
#define PAPIF_DMEM_PAGESIZE 11
#define PAPIF_DMEM_MAXVAL  12
#define PAPI_OK            0
#define PAPI_EINVAL        -1
#define PAPI_ENOMEM        -2
#define PAPI_ESYS          -3
#define PAPI_ECMP          -4
#define PAPI_ECLOST        -5
#define PAPI_EBUG          -6
#define PAPI_ENOEVNT       -7
#define PAPI_ECNFLCT       -8
#define PAPI_ENOTRUN       -9
#define PAPI_EISRUN        -10
#define PAPI_ENOEVST       -11
#define PAPI_ENOTPRESET    -12
#define PAPI_ENOCNTR       -13
#define PAPI_EMISC         -14
#define PAPI_EPERM         -15
#define PAPI_ENOINIT       -16
#define PAPI_ENOCMP        -17
#define PAPI_ENOSUPP       -18
#define PAPI_ENOIMPL       -19
#define PAPI_EBUF          -20
#define PAPI_EINVAL_DOM    -21
#define PAPI_EATTR         -22
#define PAPI_ECOUNT        -23
#define PAPI_ECOMBO        -24

C
C  PAPI preset event values.
C

#define PAPI_L1_DCM        ((-2147483647) - 1)
#define PAPI_L1_ICM        -2147483647
#define PAPI_L2_DCM        -2147483646
#define PAPI_L2_ICM        -2147483645
#define PAPI_L3_DCM        -2147483644
#define PAPI_L3_ICM        -2147483643
#define PAPI_L1_TCM        -2147483642
#define PAPI_L2_TCM        -2147483641
#define PAPI_L3_TCM        -2147483640
#define PAPI_CA_SNP        -2147483639
#define PAPI_CA_SHR        -2147483638
#define PAPI_CA_CLN        -2147483637
#define PAPI_CA_INV        -2147483636
#define PAPI_CA_ITV        -2147483635
#define PAPI_L3_LDM        -2147483634
#define PAPI_L3_STM        -2147483633
#define PAPI_BRU_IDL       -2147483632
#define PAPI_FXU_IDL       -2147483631
#define PAPI_FPU_IDL       -2147483630
#define PAPI_LSU_IDL       -2147483629
#define PAPI_TLB_DM        -2147483628
#define PAPI_TLB_IM        -2147483627
#define PAPI_TLB_TL        -2147483626
#define PAPI_L1_LDM        -2147483625
#define PAPI_L1_STM        -2147483624
#define PAPI_L2_LDM        -2147483623
#define PAPI_L2_STM        -2147483622
#define PAPI_BTAC_M        -2147483621
#define PAPI_PRF_DM        -2147483620
#define PAPI_L3_DCH        -2147483619
#define PAPI_TLB_SD        -2147483618
#define PAPI_CSR_FAL       -2147483617
#define PAPI_CSR_SUC       -2147483616
#define PAPI_CSR_TOT       -2147483615
#define PAPI_MEM_SCY       -2147483614
#define PAPI_MEM_RCY       -2147483613
#define PAPI_MEM_WCY       -2147483612
#define PAPI_STL_ICY       -2147483611
#define PAPI_FUL_ICY       -2147483610
#define PAPI_STL_CCY       -2147483609
#define PAPI_FUL_CCY       -2147483608
#define PAPI_HW_INT        -2147483607
#define PAPI_BR_UCN        -2147483606
#define PAPI_BR_CN         -2147483605
#define PAPI_BR_TKN        -2147483604
#define PAPI_BR_NTK        -2147483603
#define PAPI_BR_MSP        -2147483602
#define PAPI_BR_PRC        -2147483601
#define PAPI_FMA_INS       -2147483600
#define PAPI_TOT_IIS       -2147483599
#define PAPI_TOT_INS       -2147483598
#define PAPI_INT_INS       -2147483597
#define PAPI_FP_INS        -2147483596
#define PAPI_LD_INS        -2147483595
#define PAPI_SR_INS        -2147483594
#define PAPI_BR_INS        -2147483593
#define PAPI_VEC_INS       -2147483592
#define PAPI_RES_STL       -2147483591
#define PAPI_FP_STAL       -2147483590
#define PAPI_TOT_CYC       -2147483589
#define PAPI_LST_INS       -2147483588
#define PAPI_SYC_INS       -2147483587
#define PAPI_L1_DCH        -2147483586
#define PAPI_L2_DCH        -2147483585
#define PAPI_L1_DCA        -2147483584
#define PAPI_L2_DCA        -2147483583
#define PAPI_L3_DCA        -2147483582
#define PAPI_L1_DCR        -2147483581
#define PAPI_L2_DCR        -2147483580
#define PAPI_L3_DCR        -2147483579
#define PAPI_L1_DCW        -2147483578
#define PAPI_L2_DCW        -2147483577
#define PAPI_L3_DCW        -2147483576
#define PAPI_L1_ICH        -2147483575
#define PAPI_L2_ICH        -2147483574
#define PAPI_L3_ICH        -2147483573
#define PAPI_L1_ICA        -2147483572
#define PAPI_L2_ICA        -2147483571
#define PAPI_L3_ICA        -2147483570
#define PAPI_L1_ICR        -2147483569
#define PAPI_L2_ICR        -2147483568
#define PAPI_L3_ICR        -2147483567
#define PAPI_L1_ICW        -2147483566
#define PAPI_L2_ICW        -2147483565
#define PAPI_L3_ICW        -2147483564
#define PAPI_L1_TCH        -2147483563
#define PAPI_L2_TCH        -2147483562
#define PAPI_L3_TCH        -2147483561
#define PAPI_L1_TCA        -2147483560
#define PAPI_L2_TCA        -2147483559
#define PAPI_L3_TCA        -2147483558
#define PAPI_L1_TCR        -2147483557
#define PAPI_L2_TCR        -2147483556
#define PAPI_L3_TCR        -2147483555
#define PAPI_L1_TCW        -2147483554
#define PAPI_L2_TCW        -2147483553
#define PAPI_L3_TCW        -2147483552
#define PAPI_FML_INS       -2147483551
#define PAPI_FAD_INS       -2147483550
#define PAPI_FDV_INS       -2147483549
#define PAPI_FSQ_INS       -2147483548
#define PAPI_FNV_INS       -2147483547
#define PAPI_FP_OPS        -2147483546
#define PAPI_SP_OPS        -2147483545
#define PAPI_DP_OPS        -2147483544
#define PAPI_VEC_SP        -2147483543
#define PAPI_VEC_DP        -2147483542
#define PAPI_REF_CYC       -2147483541
