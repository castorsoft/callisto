-- Table: public.documentos_fiscais_arquivos

DROP TABLE IF EXISTS public.documentos_fiscais_arquivos CASCADE;

CREATE TABLE IF NOT EXISTS public.documentos_fiscais_arquivos
(
    id SERIAL,
    uuid uuid NOT NULL DEFAULT gen_random_uuid(),
    id_documento integer NOT NULL,
    conteudo_xml text COLLATE pg_catalog."default" NOT NULL,
    tipo_arquivo character varying(30) NOT NULL DEFAULT 'DOCUMENTO_FISCAL',
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT documentos_fiscais_arquivos_pkey PRIMARY KEY (id),
    CONSTRAINT fk_documentos_fiscais_arquivos_documento FOREIGN KEY (id_documento)
        REFERENCES public.documentos_fiscais (id)
        ON UPDATE CASCADE
        ON DELETE CASCADE
)

TABLESPACE pg_default;

CREATE INDEX IF NOT EXISTS idx_doc_fiscais_arquivos_id_documento
    ON public.documentos_fiscais_arquivos (id_documento);