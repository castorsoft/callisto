ALTER TABLE public.documentos_fiscais_itens
ADD COLUMN IF NOT EXISTS codigo_barras character varying(60);

