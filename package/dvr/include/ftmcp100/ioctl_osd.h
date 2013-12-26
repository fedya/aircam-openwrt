#ifndef _IOCTL_OSD_H_
	#define _IOCTL_OSD_H_

	/*
		Version 1.00: init. version

	 */
	#define OSD_VER					0x00010000
	#define OSD_VER_MAJOR	(OSD_VER >> 16)
	#define OSD_VER_MINOR		((OSD_VER & 0xffff) >> 4)
	#define OSD_VER_MINOR2	(OSD_VER & 0x000f)

	#define FMCP_IOCTL_OSD_INIT 					0x4190
	#define FMCP_IOCTL_OSD_ALPHABLEND	0x4191

	#define FMCP_OSD_DEV  "/dev/fmcp_osd"

    /* Structure used to pass a frame to the osd */
	typedef struct {
		unsigned int bg_va;	 		// input background buffer virtual addr.
												// output buffer virtual addr.
		unsigned int bg_width;		// width of background
		unsigned int bg_height;	// width of background
		unsigned int obj_width; 	// width of object
		unsigned int obj_height;	// width of object
		unsigned int obj_x;			// [in] x axis, object will be put at this offset of background, unit: pel
		unsigned int obj_y;			// [in] y axis, object will be put at this offset of background, unit: pel
		unsigned int obj_va;		// object virtual addr.
		unsigned int alpha;			// only support 0 ~ 4
												// means: 0/4, 1/4, 2/4, 3/4, 4/4
        unsigned int magic_number;
	} FMCP_OSD_PARAM;
#endif
