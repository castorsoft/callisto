CREATE TABLE public.xmls (
	id serial4 NOT NULL,
	"uuid" uuid DEFAULT gen_random_uuid() NOT NULL,
	id_arquivo int4 NULL,
	"xml" text NOT NULL,
	chave_acesso varchar(44) NOT NULL,
	tipo varchar(64) NOT NULL,
	CONSTRAINT xmls_chave_tipo_unique UNIQUE (chave_acesso, tipo),
	CONSTRAINT xmls_pk PRIMARY KEY (id),
	CONSTRAINT xmls_arquivos_fk FOREIGN KEY (id_arquivo) REFERENCES public.arquivos(id)
);