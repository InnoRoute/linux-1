#ifdef CONFIG_PANEL_NEC_NL8060BC31
static struct omap_dss_device mmi4_nl8060bc31_device = {
	.name			= "nec-nl8060bc31",
	.driver_name		= "nec-nl8060bc31",
	.type			= OMAP_DISPLAY_TYPE_DPI,
	.phy.dpi.data_lines	= 24,
};
#endif

#ifdef CONFIG_PANEL_NEC_NL10260BC19
static struct omap_dss_device mmi4_nl10260bc19_device = {
	.name			= "nec-nl10260bc19",
	.driver_name		= "nec-nl10260bc19",
	.type			= OMAP_DISPLAY_TYPE_DPI,
	.phy.dpi.data_lines	= 24,
};
#endif

#ifdef CONFIG_PANEL_NEC_NL10276BC30
static struct omap_dss_device mmi4_nl10276bc30_device = {
	.name			= "nec-nl10276bc30",
	.driver_name		= "nec-nl10276bc30",
	.type			= OMAP_DISPLAY_TYPE_DPI,
	.phy.dpi.data_lines	= 24,
};
#endif

#ifdef CONFIG_PANEL_NEC_NL10276BC20
static struct omap_dss_device mmi4_nl10276bc20_device = {
	.name			= "nec-nl10276bc20",
	.driver_name		= "nec-nl10276bc20",
	.type			= OMAP_DISPLAY_TYPE_DPI,
	.phy.dpi.data_lines	= 24,
};
#endif

#ifdef CONFIG_PANEL_NEC_NL6448BC20
static struct omap_dss_device mmi4_nl6448bc20_device = {
	.name			= "nec-nl6448bc20",
	.driver_name		= "nec-nl6448bc20",
	.type			= OMAP_DISPLAY_TYPE_DPI,
	.phy.dpi.data_lines	= 24,
};
#endif

#ifdef CONFIG_PANEL_SHARP_LQ190E1LW01
static struct omap_dss_device mmi4_lq190e1lw01_device= {
	.name			= "sharp-lq190e1lw01",
	.driver_name		= "sharp-lq190e1lw01",
	.type			= OMAP_DISPLAY_TYPE_DPI,
	.phy.dpi.data_lines	= 24,
};
#endif

#ifdef CONFIG_PANEL_KYOCERA_TCG085WVLB
static struct omap_dss_device mmi4_tcg085wvlb_device = {
	.name			= "kyocera-tcg085wvlb",
	.driver_name		= "kyocera-tcg085wvlb",
	.type			= OMAP_DISPLAY_TYPE_DPI,
	.phy.dpi.data_lines	= 24,
};
#endif

#ifdef CONFIG_PANEL_KYOCERA_TCG057VG1AC
static struct omap_dss_device mmi4_tcg057vg1ac_device = {
	.name			= "kyocera-tcg057vg1ac",
	.driver_name		= "kyocera-tcg057vg1ac",
	.type			= OMAP_DISPLAY_TYPE_DPI,
	.phy.dpi.data_lines	= 24,
};
#endif

#ifdef CONFIG_PANEL_KYOCERA_TCG057VGLBA
static struct omap_dss_device mmi4_tcg057vglba_device = {
	.name			= "kyocera-tcg057vglba",
	.driver_name		= "kyocera-tcg057vglba",
	.type			= OMAP_DISPLAY_TYPE_DPI,
	.phy.dpi.data_lines	= 24,
};
#endif

#ifdef CONFIG_PANEL_MITSUBISHI_K407W
static struct omap_dss_device mmi4_k407w_device = {
	.name			= "mitsubishi-k407w",
	.driver_name		= "mitsubishi-k407w",
	.type			= OMAP_DISPLAY_TYPE_DPI,
	.phy.dpi.data_lines	= 24,
};
#endif

#ifdef CONFIG_PANEL_MITSUBISHI_T909F
static struct omap_dss_device mmi4_t909f_device = {
	.name			= "mitsubishi-t909f",
	.driver_name		= "mitsubishi-t909f",
	.type			= OMAP_DISPLAY_TYPE_DPI,
	.phy.dpi.data_lines	= 24,
};
#endif

#ifdef CONFIG_PANEL_MITSUBISHI_AA084VG01
static struct omap_dss_device mmi4_aa084vg01_device = {
       .name                   = "mitsubishi-aa084vg01",
       .driver_name            = "mitsubishi-aa084vg01",
       .type                   = OMAP_DISPLAY_TYPE_DPI,
       .phy.dpi.data_lines     = 24,
};
#endif

#ifdef CONFIG_PANEL_MITSUBISHI_AA104XD02
static struct omap_dss_device mmi4_aa104xd02_device = {
        .name                   = "mitsubishi-aa104xd02",
        .driver_name            = "mitsubishi-aa104xd02",
        .type                   = OMAP_DISPLAY_TYPE_DPI,
        .phy.dpi.data_lines     = 24,
};
#endif

#ifdef CONFIG_PANEL_MITSUBISHI_AA084SB01
static struct omap_dss_device mmi4_aa084sb01_device = {
	        .name                   = "mitsubishi-aa084sb01",
	        .driver_name            = "mitsubishi-aa084sb01",
	        .type                   = OMAP_DISPLAY_TYPE_DPI,
	        .phy.dpi.data_lines     = 24,
};
#endif

#ifdef CONFIG_PANEL_MITSUBISHI_AA104SH02
static struct omap_dss_device mmi4_aa104sh02_device = {
	        .name                   = "mitsubishi-aa104sh02",
	        .driver_name            = "mitsubishi-aa104sh02",
	        .type                   = OMAP_DISPLAY_TYPE_DPI,
	        .phy.dpi.data_lines     = 24,
};
#endif

#ifdef CONFIG_OMAP2_DSS_VENC
static struct omap_dss_device mmi4_tv_device = {
	.name			= "tv",
	.driver_name		= "venc",
	.type			= OMAP_DISPLAY_TYPE_VENC,
	.phy.venc.type		= OMAP_DSS_VENC_TYPE_SVIDEO,
	.phy.venc.invert_polarity = true,
	.phy.venc.bypass_tvout  = true,
};
#endif

#ifdef CONFIG_PANEL_CHIMEI_G070Y2L01
static struct omap_dss_device mmi4_g070y2l01_device = {
	.name			= "chimei-g070y2l01",
	.driver_name		= "chimei-g070y2l01",
	.type			= OMAP_DISPLAY_TYPE_DPI,
	.phy.dpi.data_lines	= 24,
};
#endif

#ifdef CONFIG_PANEL_OMAP_CMDLINE
static struct omap_dss_device mmi4_omappanel_device= {
	.name			= "omappanel",
	.driver_name		= "omappanel",
	.type			= OMAP_DISPLAY_TYPE_DPI,
	.phy.dpi.data_lines	= 24,
};
#endif

static struct omap_dss_device *mmi4_dss_devices[] = {
#ifdef CONFIG_PANEL_NEC_NL8060BC31
	&mmi4_nl8060bc31_device,
#endif
#ifdef CONFIG_PANEL_NEC_NL10260BC19
	&mmi4_nl10260bc19_device,
#endif
#ifdef CONFIG_PANEL_NEC_NL10276BC30
	&mmi4_nl10276bc30_device,
#endif
#ifdef CONFIG_PANEL_NEC_NL10276BC20
	&mmi4_nl10276bc20_device,
#endif
#ifdef CONFIG_PANEL_NEC_NL6448BC20
	&mmi4_nl6448bc20_device,
#endif
#ifdef CONFIG_PANEL_SHARP_LQ190E1LW01
	&mmi4_lq190e1lw01_device,
#endif
#ifdef CONFIG_PANEL_KYOCERA_TCG085WVLB
	&mmi4_tcg085wvlb_device,
#endif
#ifdef CONFIG_PANEL_KYOCERA_TCG057VG1AC
	&mmi4_tcg057vg1ac_device,
#endif
#ifdef CONFIG_PANEL_KYOCERA_TCG057VGLBA
	&mmi4_tcg057vglba_device,
#endif
#ifdef CONFIG_PANEL_MITSUBISHI_K407W
	&mmi4_k407w_device,
#endif
#ifdef CONFIG_PANEL_MITSUBISHI_T909F
	&mmi4_t909f_device,
#endif
#ifdef CONFIG_PANEL_MITSUBISHI_AA084VG01
        &mmi4_aa084vg01_device,
#endif
#ifdef CONFIG_PANEL_MITSUBISHI_AA104XD02
	&mmi4_aa104xd02_device,
#endif
#ifdef CONFIG_PANEL_MITSUBISHI_AA084SB01
        &mmi4_aa084sb01_device,
#endif
#ifdef CONFIG_PANEL_MITSUBISHI_AA104SH02
        &mmi4_aa104sh02_device,
#endif		
#ifdef CONFIG_PANEL_CHIMEI_G070Y2L01
        &mmi4_g070y2l01_device,
#endif
#ifdef CONFIG_OMAP2_DSS_VENC
	&mmi4_tv_device,
#endif
#ifdef CONFIG_PANEL_OMAP_CMDLINE
	&mmi4_omappanel_device,
#endif
};
#ifdef CONFIG_PANEL_MITSUBISHI_AA104XD02
# define MMI4_DSS_DEFAULT_PANEL	&mmi4_aa104xd02_device
#else
# define MMI4_DSS_DEFAULT_PANEL	NULL
#endif
