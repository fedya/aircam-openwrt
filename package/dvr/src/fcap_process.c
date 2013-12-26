#include <linux/string.h>
#include <linux/kernel.h>


typedef struct drv_buf_header{
    u32 Tag;            ///< magic number => FCAP_DRV_TAG
    u32 size;           ///< header size
    u32 VBI[2];         ///< VBI enable
    u32 Y[2];           ///< buffer offset of Y
    u32 U[2];           ///< buffer offset of U
    u32 V[2];           ///< buffer offset of V
    u16 width;          ///< output target width
    u16 height;         ///< output target height
    u16 bg_x;           ///< left position of background buffer
    u16 bg_y;           ///< top position of background buffer
    u16 bg_w;           ///< width of background buffer
    u16 bg_h;           ///< height of background buffer
    int buf_info;       ///< buffer information => FCAP_BUF_INFO(i,p) for record frame/field mode, top/bottom field
    u8  ch_num;         ///< channel number => FCAP_CHANNEL_NUM(c, p, s)
} drv_buf_header_t;

static void __draw_h_line(void *start,int length)
{
memset(start,128,length);
}
static void __draw_v_line(void *start,int length,int buf_width)
{
int offset=0;
	for(offset=0;offset<length;offset++){
		*(char *)(start+buf_width*offset)=128;
	}
}
static void pcap_post(drv_buf_header_t * pbuf_header,void* par2)
{
char * pbuf=par2;
int width=pbuf_header->width/5;
int height=pbuf_header->height/5;
int x_post=(pbuf_header->width-width)/2;
int y_post=(pbuf_header->height-height)/2;

if(x_post+width>pbuf_header->width||y_post+height>pbuf_header->height){
	printk("@@Wrong h_range=%d-w_range=%d should be %d-%d\n",x_post+width,y_post+height,pbuf_header->width,pbuf_header->height);
	return;
}
int v1_offset=2*y_post*pbuf_header->width+2*x_post;
int v2_offset=2*(y_post+height)*pbuf_header->width+2*x_post;
int h_length=2*width;
int v_length=height;
__draw_h_line(pbuf+v1_offset,h_length);
__draw_h_line(pbuf+v2_offset,h_length);
__draw_v_line(pbuf+v1_offset,v_length,2*pbuf_header->width);
__draw_v_line(pbuf+v1_offset+h_length,v_length,2*pbuf_header->width);
}

int pcap_post_process(void* par1,void* par2)
{

drv_buf_header_t *pbuf_header=(drv_buf_header_t *)par1;
if(pbuf_header==0||par2==0){
	printk("@@cap buf address error\n");
	return -1;
}
	pcap_post(pbuf_header,par2);
return 0;
}
