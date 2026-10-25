-- Table: public.pessoas_contabilista

-- DROP TABLE IF EXISTS public.pessoas_contabilista;

CREATE TABLE IF NOT EXISTS public.pessoas_contabilista
(
    id SERIAL,
    id_pessoa integer NOT NULL,
    cpf character(11) COLLATE pg_catalog."default",
    crc character varying(20) COLLATE pg_catalog."default" NOT NULL,
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT pessoas_contabilista_pkey PRIMARY KEY (id, id_pessoa),
    CONSTRAINT pessoas_contabilista_pessoa FOREIGN KEY (id_pessoa)
        REFERENCES public.pessoas (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
)

TABLESPACE pg_default;