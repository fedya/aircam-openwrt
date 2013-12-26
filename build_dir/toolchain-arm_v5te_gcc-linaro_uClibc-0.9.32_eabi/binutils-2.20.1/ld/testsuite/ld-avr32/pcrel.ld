ENTRY(_start)
SECTIONS
{
	.text 0xa0000000:
	{
		*(.text)
	}

	.text.pcrel10 0xa0000400:
	{
		*(.text.pcrel10)
	}

	.text.pcrel16 0xa0008000:
	{
		*(.text.pcrel16)
	}

	.text.pcrel21 0xa0200000:
	{
		*(.text.pcrel21)
	}
}
