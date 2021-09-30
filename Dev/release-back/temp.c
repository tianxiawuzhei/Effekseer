#include <mono/metadata/mono-config.h>
#include <mono/metadata/assembly.h>

extern const unsigned char assembly_data_mqoToEffekseerModelConverter_exe [];
static const MonoBundledAssembly assembly_bundle_mqoToEffekseerModelConverter_exe = {"mqoToEffekseerModelConverter.exe", assembly_data_mqoToEffekseerModelConverter_exe, 32768};
extern const unsigned char assembly_data_mscorlib_dll [];
static const MonoBundledAssembly assembly_bundle_mscorlib_dll = {"mscorlib.dll", assembly_data_mscorlib_dll, 3026944};
extern const unsigned char assembly_data_System_dll [];
static const MonoBundledAssembly assembly_bundle_System_dll = {"System.dll", assembly_data_System_dll, 1777664};
extern const unsigned char assembly_data_Mono_Security_dll [];
static const MonoBundledAssembly assembly_bundle_Mono_Security_dll = {"Mono.Security.dll", assembly_data_Mono_Security_dll, 301568};
extern const unsigned char assembly_data_System_Configuration_dll [];
static const MonoBundledAssembly assembly_bundle_System_Configuration_dll = {"System.Configuration.dll", assembly_data_System_Configuration_dll, 128512};
extern const unsigned char assembly_data_System_Xml_dll [];
static const MonoBundledAssembly assembly_bundle_System_Xml_dll = {"System.Xml.dll", assembly_data_System_Xml_dll, 1315840};
extern const unsigned char assembly_data_System_Security_dll [];
static const MonoBundledAssembly assembly_bundle_System_Security_dll = {"System.Security.dll", assembly_data_System_Security_dll, 132608};
extern const unsigned char assembly_data_System_Core_dll [];
static const MonoBundledAssembly assembly_bundle_System_Core_dll = {"System.Core.dll", assembly_data_System_Core_dll, 736256};
extern const unsigned char assembly_data_Mono_Posix_dll [];
static const MonoBundledAssembly assembly_bundle_Mono_Posix_dll = {"Mono.Posix.dll", assembly_data_Mono_Posix_dll, 195072};

static const MonoBundledAssembly *bundled [] = {
	&assembly_bundle_mqoToEffekseerModelConverter_exe,
	&assembly_bundle_mscorlib_dll,
	&assembly_bundle_System_dll,
	&assembly_bundle_Mono_Security_dll,
	&assembly_bundle_System_Configuration_dll,
	&assembly_bundle_System_Xml_dll,
	&assembly_bundle_System_Security_dll,
	&assembly_bundle_System_Core_dll,
	&assembly_bundle_Mono_Posix_dll,
	NULL
};

static char *image_name = "mqoToEffekseerModelConverter.exe";

static void install_dll_config_files (void) {

}

static const char *config_dir = NULL;
void mono_mkbundle_init ()
{
	install_dll_config_files ();
	mono_register_bundled_assemblies(bundled);
}
int mono_main (int argc, char* argv[]);

#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

static char **mono_options = NULL;

static int count_mono_options_args (void)
{
	const char *e = getenv ("MONO_BUNDLED_OPTIONS");
	const char *p, *q;
	int i, n;

	if (e == NULL)
		return 0;

	/* Don't bother with any quoting here. It is unlikely one would
	 * want to pass options containing spaces anyway.
	 */

	p = e;
	n = 1;
	while ((q = strchr (p, ' ')) != NULL) {
		n++;
		p = q + 1;
	}

	mono_options = malloc (sizeof (char *) * (n + 1));

	p = e;
	i = 0;
	while ((q = strchr (p, ' ')) != NULL) {
		mono_options[i] = malloc ((q - p) + 1);
		memcpy (mono_options[i], p, q - p);
		mono_options[i][q - p] = '\0';
		i++;
		p = q + 1;
	}
	mono_options[i++] = strdup (p);
	mono_options[i] = NULL;

	return n;
}


int main (int argc, char* argv[])
{
	char **newargs;
	int i, k = 0;

	newargs = (char **) malloc (sizeof (char *) * (argc + 2 + count_mono_options_args ()));

	newargs [k++] = argv [0];

	if (mono_options != NULL) {
		i = 0;
		while (mono_options[i] != NULL)
			newargs[k++] = mono_options[i++];
	}

	newargs [k++] = image_name;

	for (i = 1; i < argc; i++) {
		newargs [k++] = argv [i];
	}
	newargs [k] = NULL;
	
	if (config_dir != NULL && getenv ("MONO_CFG_DIR") == NULL)
		mono_set_dirs (getenv ("MONO_PATH"), config_dir);
	
	mono_mkbundle_init();

	return mono_main (k, newargs);
}
