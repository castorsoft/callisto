-- Table: public.produtos

-- DROP TABLE IF EXISTS public.produtos;

CREATE TABLE IF NOT EXISTS public.produtos
(
    id SERIAL,
    uuid uuid NOT NULL DEFAULT gen_random_uuid(),
    descricao character varying(120) COLLATE pg_catalog."default" NOT NULL,
    codigo_barra character varying(20) COLLATE pg_catalog."default",
    id_unidade_medida integer NOT NULL,
    tipo_atividade tipo_produto_atividade NOT NULL,
    id_ncm integer NOT NULL,
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    codigo_interno character varying(20) COLLATE pg_catalog."default",
    id_anp integer,
    CONSTRAINT produtos_pkey PRIMARY KEY (id),
    CONSTRAINT fk_produtos_anp FOREIGN KEY (id_anp)
        REFERENCES public.anp (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION,
    CONSTRAINT fk_produtos_ncm FOREIGN KEY (id_ncm)
        REFERENCES public.ncms (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION,
    CONSTRAINT fk_produtos_unidade_medida FOREIGN KEY (id_unidade_medida)
        REFERENCES public.unidades_medida (id) MATCH SIMPLE
        ON UPDATE NO ACTION
        ON DELETE NO ACTION
)

TABLESPACE pg_default;