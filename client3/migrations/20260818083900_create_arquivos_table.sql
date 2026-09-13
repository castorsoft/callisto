-- Table: public.arquivos

-- DROP TABLE IF EXISTS public.arquivos;

CREATE TABLE IF NOT EXISTS public.arquivos
(
    id serial,
    uuid uuid NOT NULL DEFAULT gen_random_uuid(),
    caminho_arquivo text COLLATE pg_catalog."default" NOT NULL,
    nome_arquivo character varying(255) COLLATE pg_catalog."default" NOT NULL,
    processado boolean NOT NULL DEFAULT false,
    possui_erro boolean NOT NULL DEFAULT false,
    mensagem_erro character varying(255) COLLATE pg_catalog."default",
    processar_novamente boolean,
    criado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em timestamp without time zone NOT NULL DEFAULT CURRENT_TIMESTAMP
)

TABLESPACE pg_default;