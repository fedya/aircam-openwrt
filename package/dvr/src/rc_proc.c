#undef RC_DEBUG
extern void rcdump (int ch, char * msg);
extern int rcdump_init (void);

#define RC_DEBUG(args...) \
{\
	char msg[0x100];\
	sprintf (msg, args);\
	rcdump(rate_control->ch, msg);\
}


#define LOG_PATH "/mnt/nfs/log%d_%02d.txt"
#define BUF_LEN (100*1024)
#define BUF_TH (90*1024)
static void log_file(void *param);
typedef struct {
	int ch;
	int fn_n;
	struct workqueue_struct *process_wq;
	unsigned char buf[2][BUF_LEN];
	int offset[2];
	int w_buffer;
	int r_buffer;
} dumpb;
dumpb *dump[16] = {NULL};
static DECLARE_WORK(work_log, (void *)log_file, 0);
static int dump_ch = 0;
void log_file(void *param)
{
	unsigned char fn[0x80];
	dumpb * db = (dumpb *)param;
	int fd1, ret;
	mm_segment_t fs;

	fs = get_fs();
	set_fs(KERNEL_DS);

	sprintf(fn, LOG_PATH, db->ch, db->fn_n ++);
	printk ("Write Log: %s, len = 0x%x\n", fn, db->offset[db->r_buffer]);
	fd1 = sys_open(fn, O_WRONLY|O_CREAT, 0777);
	if(fd1 < 0) {
		printk ("Error to open %s\n", fn);
		return;
	}

	ret = sys_write(fd1, db->buf[db->r_buffer], db->offset[db->r_buffer]);
	if(ret <= 0)
		printk("Error to write %s\n", fn);
	db->offset[db->r_buffer] = 0;
	printk ("log from %d\n", db->r_buffer);
	sys_close(fd1);
	set_fs(fs);
}

int rcdump_init (void)
{
	int ch = dump_ch;
	if (ch >= 16) {
		printk ("rc_init fail %d\n", ch);
		goto rcdump_fail;
	}
	dump[ch] = vmalloc(sizeof(dumpb));
	if(dump == NULL) {
		printk ("vmalloc dump_buf %d error", ch);
		goto rcdump_fail;
	}
	memset (dump[ch], 0, sizeof(dumpb));
	dump[ch]->process_wq = create_workqueue("logstorage");
	dump_ch ++;
	return ch;
rcdump_fail:
	return -1;
}

void rcdump (int ch, char * msg)
{
	int n = strlen(msg);
	dumpb * db = dump[ch];
	if (NULL == db) {
		printk ("rcdump: no this channel %d\n", ch);
		return;
	}
	if ((db->offset[db->w_buffer] + n) > BUF_LEN) {
		printk ("error1, off: %d, n = %d\n", db->offset[db->w_buffer], n);
		while (1);
	}
	if (n >= 0x100) {
		printk ("error2, off: %d, n = %d\n", db->offset[db->w_buffer], n);
		while (1);
	}
	strncpy (&db->buf[db->w_buffer][db->offset[db->w_buffer]], msg, n);
	db->offset[db->w_buffer] += n;
	if (db->offset[db->w_buffer] > BUF_TH) {
		db->r_buffer = db->w_buffer;
		if (0 == db->w_buffer)
			db->w_buffer = 1;
		else
			db->w_buffer = 0;
		printk ("change to %d\n", db->w_buffer);
		db->offset[db->w_buffer] = 0;
		PREPARE_WORK(&work_log,(void *)log_file,(void *)db);
		queue_delayed_work(db->process_wq, &work_log, 0);
	}
}

