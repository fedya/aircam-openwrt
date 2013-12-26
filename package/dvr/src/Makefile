MODULE0 = dvr_disp
MODULE1 = dvr_enc
MODULE2 = dvr_dec
MODULE3 = dvr_common

$(MODULE0)-objs := disp_main.o
$(MODULE1)-objs := enc_main.o dataout_entity.o mcp210_process.o h264e_ratecontrol.o mcp100_process.o mp4f_ratecontrol.o fcap_process.o
$(MODULE2)-objs := dec_main.o datain_entity.o
$(MODULE3)-objs := util_cfg.o nodirty.o dvr_graph.o

obj-m += $(MODULE3).o $(MODULE0).o $(MODULE1).o $(MODULE2).o
obj-m := $(sort $(obj-m))

tool:
	$(CC)  dvr_api_offset.c -o dvr_api_offset
