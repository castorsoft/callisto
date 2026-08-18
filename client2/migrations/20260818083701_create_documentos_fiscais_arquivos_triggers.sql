-- FUNCTION: public.fn_documentos_fiscais_arquivos_bu0()

-- DROP FUNCTION IF EXISTS public.fn_documentos_fiscais_arquivos_bu0();

CREATE OR REPLACE FUNCTION public.fn_documentos_fiscais_arquivos_bu0()
    RETURNS trigger
    LANGUAGE 'plpgsql'
    COST 100
    VOLATILE NOT LEAKPROOF
AS $BODY$
	BEGIN
		NEW.atualizado_em := CURRENT_TIMESTAMP;
		RETURN NEW;
	END;
$BODY$;

-- Trigger: documentos_fiscais_arquivos_bu0

-- DROP TRIGGER IF EXISTS documentos_fiscais_arquivos_bu0 ON public.documentos_fiscais_arquivos;

CREATE OR REPLACE TRIGGER documentos_fiscais_arquivos_bu0
    BEFORE UPDATE
    ON public.documentos_fiscais_arquivos
    FOR EACH ROW
    EXECUTE FUNCTION public.fn_documentos_fiscais_arquivos_bu0();