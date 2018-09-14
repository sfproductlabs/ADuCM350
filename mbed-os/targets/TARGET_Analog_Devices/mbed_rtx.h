/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MBED_MBED_RTX_H
#define MBED_MBED_RTX_H

#include <stdint.h>

#ifndef INITIAL_SP
#if defined(TARGET_EV_COG_AD3029LZ)

#define INITIAL_SP              (0x20004000UL)

#elif defined(TARGET_EV_COG_AD4050LZ)

#define INITIAL_SP              (0x20048000UL)

#elif defined(TARGET_EVAL_ADUCM350EBZ)

#define INITIAL_SP              (0x20004000UL)

#else
#error "INITIAL_SP is not defined for this target in the mbed_rtx.h file"
#endif
#endif

#ifndef OS_CLOCK
#if defined(TARGET_EV_COG_AD3029LZ)||\
       defined(TARGET_EV_COG_AD4050LZ)

#define OS_CLOCK                26000000

#elif defined(TARGET_EVAL_ADUCM350EBZ)

#define OS_CLOCK                16000000

#else
#warning "OS_CLOCK  is not defined for this target in the mbed_rtx.h file"
#endif
#endif



#endif  // MBED_MBED_RTX_H
