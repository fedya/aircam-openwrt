#include <stdio.h>
#include <sys/types.h>
#include "dvr_common_api.h"
#include "dvr_disp_api.h"
#include "dvr_enc_api.h"
#include "dvr_dec_api.h"



void common_api(void)
{
}

void disp_api(void)
{
    int num;
    printf("Select ioctl:\n");
    printf("  4: DVR_DISP_GET_DISP_PARAM\n");
    printf("  5: DVR_DISP_SET_DISP_PARAM\n");
    printf("  6: DVR_DISP_UPDATE_DISP_PARAM\n");
    printf("  7: DVR_DISP_GET_PLANE_PARAM\n");
    printf("  8: DVR_DISP_SET_PLANE_PARAM\n");
    printf("  9: DVR_DISP_UPDATE_PLANE_PARAM\n");
    printf(" 10: DVR_DISP_CONTROL\n");
    printf(" 11: DVR_DISP_CLEAR_WIN\n");
    printf(" ==> ");
    scanf("%d", &num);
    switch(num)
    {
      case 4:
      case 5:
      {
        dvr_disp_disp_param test;
        printf(" dvr_disp_disp_param.color_attrib : 0x%x \n", ((int)&test.color_attrib) - ((int)&test) );
        printf(" dvr_disp_disp_param.transparent_color[0] : 0x%x \n", ((int)&test.transparent_color[0] ) - ((int)&test));
        printf(" dvr_disp_disp_param.transparent_color[1] : 0x%x \n", ((int)&test.transparent_color[1] ) - ((int)&test));
        printf(" dvr_disp_disp_param.vbi_info : 0x%x \n", ((int)&test.vbi_info) - ((int)&test));
        printf(" dvr_disp_disp_param.scl_info : 0x%x \n", ((int)&test.scl_info) - ((int)&test));
        printf(" dvr_disp_disp_param.dim : 0x%x \n", ((int)&test.dim) - ((int)&test));
        printf(" dvr_disp_disp_param.res : 0x%x \n", ((int)&test.res) - ((int)&test));
        break;
      }
      case 6:
      {
        dvr_disp_update_disp_param test;
        printf(" dvr_disp_update_disp_param.param : 0x%x \n", ((int)&test.param) - ((int)&test) );
        printf(" dvr_disp_update_disp_param.val.target_id : 0x%x \n", ((int)&test.val.target_id ) - ((int)&test));
        printf(" dvr_disp_update_disp_param.val.color_attrib : 0x%x \n", ((int)&test.val.color_attrib ) - ((int)&test));
        printf(" dvr_disp_update_disp_param.val.transparent_color[0] : 0x%x \n", ((int)&test.val.transparent_color[0]) - ((int)&test));
        printf(" dvr_disp_update_disp_param.val.vbi_info : 0x%x \n", ((int)&test.val.vbi_info) - ((int)&test));
        printf(" dvr_disp_update_disp_param.val.scl_info : 0x%x \n", ((int)&test.val.scl_info) - ((int)&test));
        printf(" dvr_disp_update_disp_param.val.res : 0x%x \n", ((int)&test.val.res) - ((int)&test));
        break;
      }
      case 7:
      case 8:
      {
        dvr_disp_plane_param test;
        printf(" dvr_disp_plane_param.param : 0x%x \n", ((int)&test.param) - ((int)&test) );
        printf(" dvr_disp_plane_param.param.win : 0x%x \n", ((int)&test.param.win ) - ((int)&test));
        break;
      }

      default:
        printf("No such ioctl number.\n");
        break;
    }
}

void enc_api(void)
{
}

void dec_api(void)
{
}


int main(int argc, char *argv[])
{
    int num;
    printf("Select API (0:common, 1:disp, 2:enc, 3:dec)\n");
    scanf("%d", &num);
    switch(num)
    {
      case 0:
        common_api();
        break;
      case 1:
        disp_api();
        break;
      case 2:
        enc_api();
        break;
      case 3:
        dec_api();
        break;
      default:
        break;
    }
    return 0;
}

