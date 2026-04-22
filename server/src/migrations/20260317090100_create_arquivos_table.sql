CREATE TABLE public.arquivos (
	id serial NOT NULL,
	"uuid" uuid DEFAULT gen_random_uuid() NOT NULL,
	dispositivo varchar(255) NOT NULL,
	caminho text NOT NULL,
	nome varchar(255) NOT NULL,
	conteudo text NOT NULL,
	CONSTRAINT arquivos_pk PRIMARY KEY (id)
);