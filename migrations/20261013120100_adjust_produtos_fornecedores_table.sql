-- Ajuste na tabela produtos_fornecedores:
-- 1. Redirecionar FK id_fornecedor para public.pessoas(id)
-- 2. Alterar Primary Key para a coluna 'id'
-- 3. Adicionar constraint UNIQUE em (id_fornecedor, codigo_fornecedor)
-- 4. Definir default 1.0 para multiplicador

ALTER TABLE public.produtos_fornecedores
DROP CONSTRAINT IF EXISTS fk_produtos_fornecedores_fornecedor;

ALTER TABLE public.produtos_fornecedores
ADD CONSTRAINT fk_produtos_fornecedores_fornecedor
FOREIGN KEY (id_fornecedor) REFERENCES public.pessoas (id)
ON UPDATE NO ACTION ON DELETE CASCADE;

ALTER TABLE public.produtos_fornecedores
DROP CONSTRAINT IF EXISTS produtos_fornecedores_pkey;

ALTER TABLE public.produtos_fornecedores
ADD CONSTRAINT produtos_fornecedores_pkey PRIMARY KEY (id);

ALTER TABLE public.produtos_fornecedores
ALTER COLUMN multiplicador SET DEFAULT 1.0;

ALTER TABLE public.produtos_fornecedores
DROP CONSTRAINT IF EXISTS unq_produtos_fornecedores_fornecedor_codigo;

ALTER TABLE public.produtos_fornecedores
ADD CONSTRAINT unq_produtos_fornecedores_fornecedor_codigo
UNIQUE (id_fornecedor, codigo_fornecedor);

