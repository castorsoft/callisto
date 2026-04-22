CREATE TABLE public.documentos (
	id serial NOT NULL,
	"uuid" uuid DEFAULT gen_random_uuid() NOT NULL,
	criado_em timestamp DEFAULT CURRENT_TIMESTAMP NOT NULL,
	atualizado_em timestamp DEFAULT CURRENT_TIMESTAMP NOT NULL,
	id_xml int4 NOT NULL,
	CONSTRAINT documentos_xml_unique UNIQUE (id_xml),
	CONSTRAINT documentos_pk PRIMARY KEY (id),
	CONSTRAINT documentos_xmls_fk FOREIGN KEY (id_xml) REFERENCES public.xmls(id)
);
