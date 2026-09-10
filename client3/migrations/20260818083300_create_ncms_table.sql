-- Table: public.ncms

-- DROP TABLE IF EXISTS public.ncms;

CREATE TABLE IF NOT EXISTS public.ncms
(
    id SERIAL,
    uuid uuid NOT NULL DEFAULT gen_random_uuid(),
    codigo character varying(11) COLLATE pg_catalog."default" NOT NULL,
    descricao text COLLATE pg_catalog."default" NOT NULL,
    data_vigencia_inicio date NOT NULL,
    data_vigencia_fim date,
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    id_pai integer,
    CONSTRAINT ncms_pkey PRIMARY KEY (id)
)

TABLESPACE pg_default;