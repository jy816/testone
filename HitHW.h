#ifndef HITHW_H
#define HITHW_H

/* VME 控制器寄存器映射 */

#define VIACKR1	0x153
#define VIACKR2	0x155
#define VIACKR3	0x157
#define VIACKR4	0x159
#define VIACKR5	0x15B
#define VIACKR6	0x15D
#define VIACKR7	0x15F
#define WCR		0x160
#define NMITCR	0x161
#define FVPPSR	0x162
#define EECR		0x163
#define VDPAR		0x164
#define VIMR		0x165
#define VGCR1		0x166
#define VIPR		0x167
#define LEDCR		0x168
#define LOCKR		0x169
#define VGCR2		0x16A
#define INTR		0x300				/*中断设置寄存器*/

#define VME_SEGMENT 0xd0000
#define IRQ_NUM		0x05

#endif // HITHW_H
