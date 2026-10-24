-- Table: public.unidades_medida

-- DROP TABLE IF EXISTS public.unidades_medida;

CREATE TABLE IF NOT EXISTS public.unidades_medida
(
    id SERIAL,
    uuid uuid NOT NULL DEFAULT gen_random_uuid(),
    sigla character varying COLLATE pg_catalog."default" NOT NULL,
    descricao character varying COLLATE pg_catalog."default" NOT NULL,
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT unidades_medida_pkey PRIMARY KEY (id),
    CONSTRAINT unq_unidades_medida_sigla UNIQUE (sigla)
)

TABLESPACE pg_default;