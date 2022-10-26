//
// Created by Alessandro Budroni on 07/09/2022.
//

#ifndef FISHER_YATES_RESEARCH_H
#define FISHER_YATES_RESEARCH_H

#include <stdint.h>
#include <immintrin.h>
#include "fisher_yates.h"

#define AVX256_X_BLOCK      ((PARAM_N1)/16)

typedef union {
    __m256i avx[AVX256_X_BLOCK];
    uint16_t i[PARAM_N1];
} permAVX_t;

void perm_set_random_avx(permAVX_t *p, uint8_t seed[SEED_BYTES]);

int verify_permutation_avx(permAVX_t *p);

//static const uint32_t fisher_yates_sampling_max[] = {
//        64376, 64324, 65508, 65455, 65402, 65349, 65296, 65243, 65190, 65137, 65084, 65031, 64978, 64925, 64872, 64819, 64766, 64713, 64660, 64607, 64554, 64501, 64448, 64395, 64342, 65502, 65448, 65394, 65340, 65286, 65232, 65178, 65124, 65070, 65016, 64962, 64908, 64854, 64800, 64746, 64692, 64638, 64584, 64530, 64476, 64422, 64368, 65505, 65450, 65395, 65340, 65285, 65230, 65175, 65120, 65065, 65010, 64955, 64900, 64845, 64790, 64735, 64680, 64625, 64570, 64515, 64460, 64405, 65520, 65464, 65408, 65352, 65296, 65240, 65184, 65128, 65072, 65016, 64960, 64904, 64848, 64792, 64736, 64680, 64624, 64568, 64512, 64456, 64400, 65493, 65436, 65379, 65322, 65265, 65208, 65151, 65094, 65037, 64980, 64923, 64866, 64809, 64752, 64695, 64638, 64581, 64524, 64467, 64410, 65482, 65424, 65366, 65308, 65250, 65192, 65134, 65076, 65018, 64960, 64902, 64844, 64786, 64728, 64670, 64612, 64554, 64496, 64438, 65490, 65431, 65372, 65313, 65254, 65195, 65136, 65077, 65018, 64959, 64900, 64841, 64782, 64723, 64664, 64605, 64546, 64487, 65520, 65460, 65400, 65340, 65280, 65220, 65160, 65100, 65040, 64980, 64920, 64860, 64800, 64740, 64680, 64620, 64560, 64500, 65514, 65453, 65392, 65331, 65270, 65209, 65148, 65087, 65026, 64965, 64904, 64843, 64782, 64721, 64660, 64599, 64538, 65534, 65472, 65410, 65348, 65286, 65224, 65162, 65100, 65038, 64976, 64914, 64852, 64790, 64728, 64666, 64604, 64542, 65520, 65457, 65394, 65331, 65268, 65205, 65142, 65079, 65016, 64953, 64890, 64827, 64764, 64701, 64638, 64575, 65536, 65472, 65408, 65344, 65280, 65216, 65152, 65088, 65024, 64960, 64896, 64832, 64768, 64704, 64640, 64576, 65520, 65455, 65390, 65325, 65260, 65195, 65130, 65065, 65000, 64935, 64870, 64805, 64740, 64675, 64610, 64545, 65472, 65406, 65340, 65274, 65208, 65142, 65076, 65010, 64944, 64878, 64812, 64746, 64680, 64614, 65526, 65459, 65392, 65325, 65258, 65191, 65124, 65057, 64990, 64923, 64856, 64789, 64722, 64655, 64588, 65484, 65416, 65348, 65280, 65212, 65144, 65076, 65008, 64940, 64872, 64804, 64736, 64668, 64600, 65481, 65412, 65343, 65274, 65205, 65136, 65067, 64998, 64929, 64860, 64791, 64722, 64653, 65520, 65450, 65380, 65310, 65240, 65170, 65100, 65030, 64960, 64890, 64820, 64750, 64680, 65533, 65462, 65391, 65320, 65249, 65178, 65107, 65036, 64965, 64894, 64823, 64752, 64681, 65520, 65448, 65376, 65304, 65232, 65160, 65088, 65016, 64944, 64872, 64800, 64728, 64656, 65481, 65408, 65335, 65262, 65189, 65116, 65043, 64970, 64897, 64824, 64751, 64678, 65490, 65416, 65342, 65268, 65194, 65120, 65046, 64972, 64898, 64824, 64750, 64676, 65475, 65400, 65325, 65250, 65175, 65100, 65025, 64950, 64875, 64800, 64725, 65512, 65436, 65360, 65284, 65208, 65132, 65056, 64980, 64904, 64828, 64752, 65527, 65450, 65373, 65296, 65219, 65142, 65065, 64988, 64911, 64834, 64757, 65520, 65442, 65364, 65286, 65208, 65130, 65052, 64974, 64896, 64818, 64740, 65491, 65412, 65333, 65254, 65175, 65096, 65017, 64938, 64859, 64780, 65520, 65440, 65360, 65280, 65200, 65120, 65040, 64960, 64880, 64800, 65529, 65448, 65367, 65286, 65205, 65124, 65043, 64962, 64881, 64800, 65518, 65436, 65354, 65272, 65190, 65108, 65026, 64944, 64862, 64780, 65487, 65404, 65321, 65238, 65155, 65072, 64989, 64906, 64823, 65520, 65436, 65352, 65268, 65184, 65100, 65016, 64932, 64848, 65535, 65450, 65365, 65280, 65195, 65110, 65025, 64940, 64855, 65532, 65446, 65360, 65274, 65188, 65102, 65016, 64930, 64844, 65511, 65424, 65337, 65250, 65163, 65076, 64989, 64902, 64815, 65472, 65384, 65296, 65208, 65120, 65032, 64944, 64856, 65504, 65415, 65326, 65237, 65148, 65059, 64970, 64881, 65520, 65430, 65340, 65250, 65160, 65070, 64980, 64890, 65520, 65429, 65338, 65247, 65156, 65065, 64974, 64883, 65504, 65412, 65320, 65228, 65136, 65044, 64952, 64860, 65472, 65379, 65286, 65193, 65100, 65007, 64914, 65518, 65424, 65330, 65236, 65142, 65048, 64954, 64860, 65455, 65360, 65265, 65170, 65075, 64980, 64885, 65472, 65376, 65280, 65184, 65088, 64992, 64896, 65475, 65378, 65281, 65184, 65087, 64990, 64893, 65464, 65366, 65268, 65170, 65072, 64974, 64876, 65439, 65340, 65241, 65142, 65043, 64944, 65500, 65400, 65300, 65200, 65100, 65000, 64900, 65448, 65347, 65246, 65145, 65044, 64943, 65484, 65382, 65280, 65178, 65076, 64974, 65508, 65405, 65302, 65199, 65096, 64993, 65520, 65416, 65312, 65208, 65104, 65000, 65520, 65415, 65310, 65205, 65100, 64995, 65508, 65402, 65296, 65190, 65084, 64978, 65484, 65377, 65270, 65163, 65056, 64949, 65448, 65340, 65232, 65124, 65016, 65509, 65400, 65291, 65182, 65073, 64964, 65450, 65340, 65230, 65120, 65010, 65490, 65379, 65268, 65157, 65046, 65520, 65408, 65296, 65184, 65072, 64960, 65427, 65314, 65201, 65088, 64975, 65436, 65322, 65208, 65094, 64980, 65435, 65320, 65205, 65090, 64975, 65424, 65308, 65192, 65076, 65520, 65403, 65286, 65169, 65052, 65490, 65372, 65254, 65136, 65018, 65450, 65331, 65212, 65093, 65520, 65400, 65280, 65160, 65040, 65461, 65340, 65219, 65098, 65514, 65392, 65270, 65148, 65026, 65436, 65313, 65190, 65067, 65472, 65348, 65224, 65100, 65500, 65375, 65250, 65125, 65520, 65394, 65268, 65142, 65532, 65405, 65278, 65151, 65536, 65408, 65280, 65152, 65532, 65403, 65274, 65145, 65520, 65390, 65260, 65130, 65500, 65369, 65238, 65107, 65472, 65340, 65208, 65076, 65436, 65303, 65170, 65526, 65392, 65258, 65124, 65475, 65340, 65205, 65070, 65416, 65280, 65144, 65486, 65349, 65212, 65075, 65412, 65274, 65136, 65469, 65330, 65191, 65520, 65380, 65240, 65100, 65424, 65283, 65142, 65462, 65320, 65178, 65494, 65351, 65208, 65520, 65376, 65232, 65088, 65395, 65250, 65105, 65408, 65262, 65116, 65415, 65268, 65121, 65416, 65268, 65120, 65411, 65262, 65113, 65400, 65250, 65534, 65383, 65232, 65512, 65360, 65208, 65484, 65331, 65178, 65450, 65296, 65142, 65410, 65255, 65520, 65364, 65208, 65469, 65312, 65155, 65412, 65254, 65508, 65349, 65190, 65440, 65280, 65527, 65366, 65205, 65448, 65286, 65526, 65363, 65200, 65436, 65272, 65505, 65340, 65175, 65404, 65238, 65464, 65297, 65520, 65352, 65184, 65403, 65234, 65450, 65280, 65493, 65322, 65532, 65360, 65188, 65394, 65221, 65424, 65250, 65450, 65275, 65472, 65296, 65490, 65313, 65504, 65326, 65514, 65335, 65520, 65340, 65522, 65341, 65520, 65338, 65514, 65331, 65504, 65320, 65490, 65305, 65472, 65286, 65450, 65263, 65424, 65236, 65394, 65205, 65360, 65513, 65322, 65472, 65280, 65427, 65234, 65378, 65520, 65325, 65464, 65268, 65404, 65207, 65340, 65471, 65272, 65400, 65526, 65325, 65448, 65246, 65366, 65484, 65280, 65395, 65508, 65302, 65412, 65520, 65312, 65417, 65520, 65310, 65410, 65508, 65296, 65391, 65484, 65270, 65360, 65448, 65534, 65317, 65400, 65481, 65262, 65340, 65416, 65490, 65268, 65339, 65408, 65475, 65250, 65314, 65376, 65436, 65494, 65265, 65320, 65373, 65424, 65473, 65520, 65286, 65330, 65372, 65412, 65450, 65486, 65520, 65280, 65311, 65340, 65367, 65392, 65415, 65436, 65455, 65472, 65487, 65500, 65511, 65520, 65527, 65532, 65535, 65536, 65535, 65532, 65527, 65520, 65511, 65500, 65487, 65472, 65455, 65436, 65415, 65392, 65367, 65340, 65311, 65520, 65486, 65450, 65412, 65372, 65330, 65520, 65473, 65424, 65373, 65320, 65494, 65436, 65376, 65314, 65475, 65408, 65339, 65490, 65416, 65340, 65481, 65400, 65534, 65448, 65360, 65484, 65391, 65508, 65410, 65520, 65417, 65520, 65412, 65508, 65395, 65484, 65366, 65448, 65526, 65400, 65471, 65340, 65404, 65464, 65520, 65378, 65427, 65472, 65513, 65360, 65394, 65424, 65450, 65472, 65490, 65504, 65514, 65520, 65522, 65520, 65514, 65504, 65490, 65472, 65450, 65424, 65394, 65532, 65493, 65450, 65403, 65520, 65464, 65404, 65505, 65436, 65526, 65448, 65527, 65440, 65508, 65412, 65469, 65520, 65410, 65450, 65484, 65512, 65534, 65400, 65411, 65416, 65415, 65408, 65395, 65520, 65494, 65462, 65424, 65520, 65469, 65412, 65486, 65416, 65475, 65526, 65436, 65472, 65500, 65520, 65532, 65536, 65532, 65520, 65500, 65472, 65436, 65514, 65461, 65520, 65450, 65490, 65520, 65424, 65435, 65436, 65427, 65520, 65490, 65450, 65509, 65448, 65484, 65508, 65520, 65520, 65508, 65484, 65448, 65500, 65439, 65464, 65475, 65472, 65455, 65518, 65472, 65504, 65520, 65520, 65504, 65472, 65511, 65532, 65535, 65520, 65487, 65518, 65529, 65520, 65491, 65520, 65527, 65512, 65475, 65490, 65481, 65520, 65533, 65520, 65481, 65484, 65526, 65472, 65520, 65536, 65520, 65534, 65514, 65520, 65490, 65482, 65493, 65520, 65505, 65502, 65508, 65520, 65535, 65500, 65513, 65520, 65518, 65504, 65520, 65516, 65532, 65520, 65518, 65520, 65520, 65512, 65527, 65520, 65520, 65518, 65505, 65536, 65534, 65520, 65511, 65520, 65529, 65520, 65525, 65520, 65527, 65516, 65520, 65520, 65531, 65520, 65535, 65536, 65535, 65534, 65533, 65532, 65527, 65530, 65529, 65536, 65534, 65532, 65535, 65536, 65535, 65536, 65536
//};

// AVX256_X_BLOCK-div = AVX256_X_BLOCK - ((PARAM_N1 -i -1) >> 4) = AVX256_X_BLOCK = ((PARAM_N1 -i -1) / 16)
static const uint8_t precomp_div[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,19,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,22,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,23,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,24,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,25,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,26,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,29,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,34,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,36,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,37,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,39,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,41,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,42,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,43,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,44,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,46,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,47,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,49,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,51,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,52,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,53,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,54,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,56,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,57,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,58,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,59,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,60,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,61,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,62,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,68,68,68,68,68,68,68,68,68,68,68,68,68,68,68,68,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,69,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,70,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,71,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,72,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,73,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,74,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,76,76,76,76,76,76,76,76,76,76,76,76,76,76,76,76,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,77,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,78,79,79,79,79,79,79,79,79,79,79,79,79,79,79,79,79,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,81,81,81,81,81,81,81,81,81,81,81,81,81,81,81,81,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,82,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,83,84,84,84,84,84,84,84,84,84,84,84,84,84,84,84,84,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,85,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,86,87,87,87,87,87,87,87,87,87,87,87,87,87,87,87,87,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,88,89,89,89,89,89,89,89,89,89,89,89,89,89,89,89,89,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,91,91,91,91,91,91,91,91,91,91,91,91,91,91,91,91,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,92,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,93,94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,94,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,95,96,96,96,96,96,96,96,96,96,96,96,96,96,96,96,96,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,98,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,106,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,107,108,108,108,108,108,108,108,108,108,108,108,108,108,108,108,108,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,109,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,110,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,111,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,113,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,114,115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,116,117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,117,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,118,119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,121,121,121,121,121,121,121,121,121,121,121,121,121,121,121,121,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,126,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128};

static const uint16_t precomp_split[] = {16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,48,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,64,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,96,96,96,96,96,96,96,96,96,96,96,96,96,96,96,96,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,112,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,144,144,144,144,144,144,144,144,144,144,144,144,144,144,144,144,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,160,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,176,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,192,208,208,208,208,208,208,208,208,208,208,208,208,208,208,208,208,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,256,272,272,272,272,272,272,272,272,272,272,272,272,272,272,272,272,288,288,288,288,288,288,288,288,288,288,288,288,288,288,288,288,304,304,304,304,304,304,304,304,304,304,304,304,304,304,304,304,320,320,320,320,320,320,320,320,320,320,320,320,320,320,320,320,336,336,336,336,336,336,336,336,336,336,336,336,336,336,336,336,352,352,352,352,352,352,352,352,352,352,352,352,352,352,352,352,368,368,368,368,368,368,368,368,368,368,368,368,368,368,368,368,384,384,384,384,384,384,384,384,384,384,384,384,384,384,384,384,400,400,400,400,400,400,400,400,400,400,400,400,400,400,400,400,416,416,416,416,416,416,416,416,416,416,416,416,416,416,416,416,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,432,448,448,448,448,448,448,448,448,448,448,448,448,448,448,448,448,464,464,464,464,464,464,464,464,464,464,464,464,464,464,464,464,480,480,480,480,480,480,480,480,480,480,480,480,480,480,480,480,496,496,496,496,496,496,496,496,496,496,496,496,496,496,496,496,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,560,576,576,576,576,576,576,576,576,576,576,576,576,576,576,576,576,592,592,592,592,592,592,592,592,592,592,592,592,592,592,592,592,608,608,608,608,608,608,608,608,608,608,608,608,608,608,608,608,624,624,624,624,624,624,624,624,624,624,624,624,624,624,624,624,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,640,656,656,656,656,656,656,656,656,656,656,656,656,656,656,656,656,672,672,672,672,672,672,672,672,672,672,672,672,672,672,672,672,688,688,688,688,688,688,688,688,688,688,688,688,688,688,688,688,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,704,720,720,720,720,720,720,720,720,720,720,720,720,720,720,720,720,736,736,736,736,736,736,736,736,736,736,736,736,736,736,736,736,752,752,752,752,752,752,752,752,752,752,752,752,752,752,752,752,768,768,768,768,768,768,768,768,768,768,768,768,768,768,768,768,784,784,784,784,784,784,784,784,784,784,784,784,784,784,784,784,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,800,816,816,816,816,816,816,816,816,816,816,816,816,816,816,816,816,832,832,832,832,832,832,832,832,832,832,832,832,832,832,832,832,848,848,848,848,848,848,848,848,848,848,848,848,848,848,848,848,864,864,864,864,864,864,864,864,864,864,864,864,864,864,864,864,880,880,880,880,880,880,880,880,880,880,880,880,880,880,880,880,896,896,896,896,896,896,896,896,896,896,896,896,896,896,896,896,912,912,912,912,912,912,912,912,912,912,912,912,912,912,912,912,928,928,928,928,928,928,928,928,928,928,928,928,928,928,928,928,944,944,944,944,944,944,944,944,944,944,944,944,944,944,944,944,960,960,960,960,960,960,960,960,960,960,960,960,960,960,960,960,976,976,976,976,976,976,976,976,976,976,976,976,976,976,976,976,992,992,992,992,992,992,992,992,992,992,992,992,992,992,992,992,1008,1008,1008,1008,1008,1008,1008,1008,1008,1008,1008,1008,1008,1008,1008,1008,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1024,1040,1040,1040,1040,1040,1040,1040,1040,1040,1040,1040,1040,1040,1040,1040,1040,1056,1056,1056,1056,1056,1056,1056,1056,1056,1056,1056,1056,1056,1056,1056,1056,1072,1072,1072,1072,1072,1072,1072,1072,1072,1072,1072,1072,1072,1072,1072,1072,1088,1088,1088,1088,1088,1088,1088,1088,1088,1088,1088,1088,1088,1088,1088,1088,1104,1104,1104,1104,1104,1104,1104,1104,1104,1104,1104,1104,1104,1104,1104,1104,1120,1120,1120,1120,1120,1120,1120,1120,1120,1120,1120,1120,1120,1120,1120,1120,1136,1136,1136,1136,1136,1136,1136,1136,1136,1136,1136,1136,1136,1136,1136,1136,1152,1152,1152,1152,1152,1152,1152,1152,1152,1152,1152,1152,1152,1152,1152,1152,1168,1168,1168,1168,1168,1168,1168,1168,1168,1168,1168,1168,1168,1168,1168,1168,1184,1184,1184,1184,1184,1184,1184,1184,1184,1184,1184,1184,1184,1184,1184,1184,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1200,1216,1216,1216,1216,1216,1216,1216,1216,1216,1216,1216,1216,1216,1216,1216,1216,1232,1232,1232,1232,1232,1232,1232,1232,1232,1232,1232,1232,1232,1232,1232,1232,1248,1248,1248,1248,1248,1248,1248,1248,1248,1248,1248,1248,1248,1248,1248,1248,1264,1264,1264,1264,1264,1264,1264,1264,1264,1264,1264,1264,1264,1264,1264,1264,1280,1280,1280,1280,1280,1280,1280,1280,1280,1280,1280,1280,1280,1280,1280,1280,1296,1296,1296,1296,1296,1296,1296,1296,1296,1296,1296,1296,1296,1296,1296,1296,1312,1312,1312,1312,1312,1312,1312,1312,1312,1312,1312,1312,1312,1312,1312,1312,1328,1328,1328,1328,1328,1328,1328,1328,1328,1328,1328,1328,1328,1328,1328,1328,1344,1344,1344,1344,1344,1344,1344,1344,1344,1344,1344,1344,1344,1344,1344,1344,1360,1360,1360,1360,1360,1360,1360,1360,1360,1360,1360,1360,1360,1360,1360,1360,1376,1376,1376,1376,1376,1376,1376,1376,1376,1376,1376,1376,1376,1376,1376,1376,1392,1392,1392,1392,1392,1392,1392,1392,1392,1392,1392,1392,1392,1392,1392,1392,1408,1408,1408,1408,1408,1408,1408,1408,1408,1408,1408,1408,1408,1408,1408,1408,1424,1424,1424,1424,1424,1424,1424,1424,1424,1424,1424,1424,1424,1424,1424,1424,1440,1440,1440,1440,1440,1440,1440,1440,1440,1440,1440,1440,1440,1440,1440,1440,1456,1456,1456,1456,1456,1456,1456,1456,1456,1456,1456,1456,1456,1456,1456,1456,1472,1472,1472,1472,1472,1472,1472,1472,1472,1472,1472,1472,1472,1472,1472,1472,1488,1488,1488,1488,1488,1488,1488,1488,1488,1488,1488,1488,1488,1488,1488,1488,1504,1504,1504,1504,1504,1504,1504,1504,1504,1504,1504,1504,1504,1504,1504,1504,1520,1520,1520,1520,1520,1520,1520,1520,1520,1520,1520,1520,1520,1520,1520,1520,1536,1536,1536,1536,1536,1536,1536,1536,1536,1536,1536,1536,1536,1536,1536,1536,1552,1552,1552,1552,1552,1552,1552,1552,1552,1552,1552,1552,1552,1552,1552,1552,1568,1568,1568,1568,1568,1568,1568,1568,1568,1568,1568,1568,1568,1568,1568,1568,1584,1584,1584,1584,1584,1584,1584,1584,1584,1584,1584,1584,1584,1584,1584,1584,1600,1600,1600,1600,1600,1600,1600,1600,1600,1600,1600,1600,1600,1600,1600,1600,1616,1616,1616,1616,1616,1616,1616,1616,1616,1616,1616,1616,1616,1616,1616,1616,1632,1632,1632,1632,1632,1632,1632,1632,1632,1632,1632,1632,1632,1632,1632,1632,1648,1648,1648,1648,1648,1648,1648,1648,1648,1648,1648,1648,1648,1648,1648,1648,1664,1664,1664,1664,1664,1664,1664,1664,1664,1664,1664,1664,1664,1664,1664,1664,1680,1680,1680,1680,1680,1680,1680,1680,1680,1680,1680,1680,1680,1680,1680,1680,1696,1696,1696,1696,1696,1696,1696,1696,1696,1696,1696,1696,1696,1696,1696,1696,1712,1712,1712,1712,1712,1712,1712,1712,1712,1712,1712,1712,1712,1712,1712,1712,1728,1728,1728,1728,1728,1728,1728,1728,1728,1728,1728,1728,1728,1728,1728,1728,1744,1744,1744,1744,1744,1744,1744,1744,1744,1744,1744,1744,1744,1744,1744,1744,1760,1760,1760,1760,1760,1760,1760,1760,1760,1760,1760,1760,1760,1760,1760,1760,1776,1776,1776,1776,1776,1776,1776,1776,1776,1776,1776,1776,1776,1776,1776,1776,1792,1792,1792,1792,1792,1792,1792,1792,1792,1792,1792,1792,1792,1792,1792,1792,1808,1808,1808,1808,1808,1808,1808,1808,1808,1808,1808,1808,1808,1808,1808,1808,1824,1824,1824,1824,1824,1824,1824,1824,1824,1824,1824,1824,1824,1824,1824,1824,1840,1840,1840,1840,1840,1840,1840,1840,1840,1840,1840,1840,1840,1840,1840,1840,1856,1856,1856,1856,1856,1856,1856,1856,1856,1856,1856,1856,1856,1856,1856,1856,1872,1872,1872,1872,1872,1872,1872,1872,1872,1872,1872,1872,1872,1872,1872,1872,1888,1888,1888,1888,1888,1888,1888,1888,1888,1888,1888,1888,1888,1888,1888,1888,1904,1904,1904,1904,1904,1904,1904,1904,1904,1904,1904,1904,1904,1904,1904,1904,1920,1920,1920,1920,1920,1920,1920,1920,1920,1920,1920,1920,1920,1920,1920,1920,1936,1936,1936,1936,1936,1936,1936,1936,1936,1936,1936,1936,1936,1936,1936,1936,1952,1952,1952,1952,1952,1952,1952,1952,1952,1952,1952,1952,1952,1952,1952,1952,1968,1968,1968,1968,1968,1968,1968,1968,1968,1968,1968,1968,1968,1968,1968,1968,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,1984,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2000,2016,2016,2016,2016,2016,2016,2016,2016,2016,2016,2016,2016,2016,2016,2016,2016,2032,2032,2032,2032,2032,2032,2032,2032,2032,2032,2032,2032,2032,2032,2032,2032,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048,2048};


#endif //FISHER_YATES_RESEARCH_H
